#include <unistd.h>
#include <lodepng.hpp>
#include <filesystem>
#include <string>
#include <fmt/base.h>
#include <fmt/std.h>

using namespace std;
namespace fs = std::filesystem;

/**
 * lodepng doesn't really seem to provide a nice image class or anything so here
 * is one just so we can keep the dimensions and the data together without
 * hassle
 */
struct Pic {
    string const &getName() const {
        return name;
    }

    unsigned getW() const {
        return width;
    }

    unsigned getH() const {
        return height;
    }

    unsigned getMax() const {
        return max(width, height);
    }

    /**
     * Calculates the 1 dimensional index that a given pixel coordinate relates
     * to. Since pixels though consist of 4 values, this gives you the index of
     * the first one.
     * @param x is the x coordinate of the pixel you want.
     * @param y is the y coordinate of the pixel you want.
     * @return the 1 dimensional index of the pixel you are interested in if it
     *         exists, and nothing if the coordinate is not valid.
     */
    optional<vector<unsigned char>::size_type> findIndex(int x, int y) const {
        if (x < 0 || y < 0 || static_cast<unsigned>(x) >= width ||
            static_cast<unsigned>(y) >= height
        ) {
                return {};
        }
        return (y * width + x) * 4;
    }

    /**
     * Draws the data from this image onto another image. If there are some bits
     * where the two do not overlap then it just does nothing for those bits.
     * @param target is the image to draw to.
     * @param x is the left coordinate of where to start drawing on the target.
     * @param y is the top coordinate of where to start drawing on the target.
     */
    void blit(Pic &target, int x, int y) const {
        for (unsigned ix = 0; ix < width; ix++) {
            for (unsigned iy = 0; iy < height; iy++) {
                auto targetIndex = target.findIndex(x + ix, y + iy);
                auto localIndex = findIndex(ix, iy);
                if (!targetIndex || !localIndex) continue;
                for (int i = 0; i < 4; i++) {
                    target.data[*targetIndex + i] = data[*localIndex + i];
                }
            }
        }
    }

    /**
     * Saves the picture to the given file.
     * @param file is where to save.
     * @return true if all went well and false if it failed to save.
     */
    bool save(fs::path file) const {
        unsigned error = lodepng::encode(
            file,
            data,
            width,
            height
        );
        if (error) {
            fmt::print(
                "Error saving image {}: {}\n",
                file,
                lodepng_error_text(error)
            );
            return false;
        }
        return true;
    }

    /**
     * Creates a pic based on an image file.
     * @param file is the path to the file.
     * @return the file if it could be loaded.
     */
    static optional<Pic> createFromFile(fs::path file) {
        vector<unsigned char> data;
        unsigned width, height;
        unsigned error = lodepng::decode(data, width, height, file);
        if (error) {
            fmt::print(
                "Error loading image {}: {}\n",
                file,
                lodepng_error_text(error)
            );
            return {};
        }
        return Pic(file.stem(), width, height, data);
    }

    /**
     * Creates a blank image with no name that is initialised as being
     * transparent.
     * @param width is the width of the image.
     * @param height is the height of the image.
     */
    static Pic createBlank(unsigned width, unsigned height) {
        vector<unsigned char>data(width * height * 4, 0);
        return Pic("", width, height, data);
    }

    private:
        string name;
        unsigned width;
        unsigned height;
        vector<unsigned char> data;

        Pic(
            string name,
            unsigned width,
            unsigned height,
            vector<unsigned char> data
        ): name(name), width(width), height(height), data(data)
        {}
};

/**
 * Represents a node in a bin packing tree.
 */
struct PackNode {
    Pic const &pic;
    unsigned const x;
    unsigned const y;
    unsigned const width;
    unsigned const height;
    PackNode *right = NULL;
    PackNode *down = NULL;

    /**
     * Yeah no default constructor you must set the picture and the boundaries.
     * @param pic is the picture the node represents.
     * @param width is the width of the boundaries this node owns.
     * @param height is the height of the boundaries this node owns.
     */
    PackNode(
        Pic const &pic,
        unsigned x,
        unsigned y,
        unsigned width,
        unsigned height
    ):
        pic(pic), x(x), y(y), width(width), height(height)
    {
        // does nothing lolle.
    }

    /**
     * Adds a new node to the tree.
     * @param child the image that the node will represent. The actual node is
     *        created when the time comes.
     * @param padding is how much padding is to be placed between items.
     * @return true if the child was successfully added as a descendant of this
     *         node, and false if it didn't fit.
     */
    bool addChild(Pic const &child, unsigned padding) {
        if (right) {
            if (right->addChild(child, padding)) return true;
        } else {
            unsigned rightWidth = pic.getW() + padding + child.getW();
            if (rightWidth <= width && child.getH() <= pic.getH()) {
                right = new PackNode(
                    child,
                    x + pic.getW() + padding,
                    y,
                    width - pic.getW() - padding,
                    pic.getH()
                );
                return true;
            }
        }
        if (down) {
            if (down->addChild(child, padding)) return true;
        } else {
            unsigned downHeight = pic.getH() + padding + child.getH();
            if (child.getW() <= width && downHeight <= height) {
                down = new PackNode(
                    child,
                    x,
                    y + pic.getH() + padding,
                    width,
                    height - pic.getH() - padding
                );
                return true;
            }
        }
        return false;
    }

    /**
     * Blits all the elements in the tree recursively onto the provided pic.
     * @param out is the place to draw the stuffs.
     */
    void flatten(Pic &out) {
        pic.blit(out, x, y);
        if (right) right->flatten(out);
        if (down) down->flatten(out);
    }
};

std::string format_as(PackNode const &tree) {
    return fmt::format(
        "({} {} {} {} {} {} {})",
        tree.pic.getName(),
        tree.x,
        tree.y,
        tree.width,
        tree.height,
        tree.down ? format_as(*tree.down) : "_",
        tree.right ? format_as(*tree.right) : "_"
    );
}

/**
 * Finds all the piccies in the given directory, loads them up, and returns them
 * in a vector. If it has trouble loading any given image then 
 */
vector<Pic> findPngs(fs::path const &path) {
    vector<Pic> pics;
    for (auto const &entry: fs::directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            optional<Pic> pic = Pic::createFromFile(entry);
            if (pic) {
                pics.push_back(pic.value());
            } else {
                fmt::print(
                    "{} failed which is bad. should we crash?\n",
                    entry.path()
                );
            }
        }
    }
    return pics;
}

/**
 * Prints how to use the program to stderr and then returns the error code that
 * the program ought now to exit with.
 * @param name is the name of the program aka argv[0] so we can print it.
 * @return program return code.
 */
int usage(char const *name) {
    fmt::print(
        "Usage: {} in_dir out_file out_dir [-x width] [-y height] [-p padding]\n",
        name
    );
    return 1;
}

int main(int argc, char **argv) {
    fs::path inFolder;
    fs::path outFile;
    fs::path outFolder;
    unsigned width = 32;
    unsigned height = 64;
    unsigned padding = 0;
    int opt;
    while ((opt = getopt(argc, argv, "x:y:p:")) != -1) {
        switch (opt) {
            case 'x':
                width = std::stoi(optarg);
                break;
            case 'y':
                height = std::stoi(optarg);
                break;
            case 'p':
                padding = std::stoi(optarg);
                break;
            default:
                return usage(argv[0]);
        }
    }
    if (argc - optind != 3) return usage(argv[0]);
    inFolder = argv[optind];
    outFile = argv[optind + 1];
    outFolder = argv[optind + 2];
    // validate that stuff exists
    fs::create_directories(outFolder);
    fs::create_directories(outFile);
    // load, sort, and pack the pictures.
    vector<Pic> pics = findPngs(inFolder);
    sort(
        pics.begin(),
        pics.end(),
        [](Pic const &a, Pic const &b) {
            return a.getMax() > b.getMax();
        }
    );
    vector<PackNode> trees;
    for (Pic const &pic: pics) {
        for (PackNode &tree: trees) {
            if (tree.addChild(pic, padding)) goto next;
        }
        trees.emplace_back(pic, 0, 0, width, height);
        next:;
    }
    // save the output.
    int i = 0;
    for (PackNode &tree: trees) {
        Pic outImage = Pic::createBlank(width, height);
        tree.flatten(outImage);
        fs::path file = outFolder / fmt::format("pack_{}.png", i);
        if (!outImage.save(file)) return 1;
        i++;
    }
    
    // ok now we need to draw all the pics to one image, then write them out in
    // some kind of binary format to a file.



    return 0;
}