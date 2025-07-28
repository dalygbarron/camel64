#include <stdio.h>
#include <unistd.h>
#include <lodepng.hpp>
#include <filesystem>
#include <string>

/**
 * lodepng doesn't really seem to provide a nice image class or anything so here
 * is one just so we can keep the dimensions and the data together without
 * hassle
 */
struct Pic {
    unsigned width, height;
    std::vector<unsigned char> data;
};

/**
 * Loads in a pic from file.
 * @param filename is the name of the file to load it from.
 * @return the file if loading was successful and otherwise nuthin.
 */
std::optional<struct Pic> loadPic(char const *filename) {
    struct Pic pic;
    unsigned error = lodepng::decode(
        pic.data,
        pic.width,
        pic.height,
        filename
    );
    if (error) {
        fprintf(
            stderr,
            "Error loading image %s: %s\n",
            filename,
            lodepng_error_text(error)
        );
        return {};
    }
    return pic;
}

/**
 * Prints how to use the program to stderr and then returns the error code that
 * the program ought now to exit with.
 * @param name is the name of the program aka argv[0] so we can print it.
 * @return program return code.
 */
int usage(char const *name) {
    fprintf(
        stderr,
        "Usage: %s in_dir out_file out_dir [-x width] [-y height] [-c]",
        name
    );
    return 1;
}

int main(int argc, char **argv) {
    char const *inFolder = NULL;
    char const *outFile = NULL;
    char const *outFolder = NULL;
    unsigned int width = 32;
    unsigned int height = 64;
    bool crashOnBadFit = true;
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
                crashOnBadFit = false;
                break;
            default:
                return usage(argv[0]);
        }
    }
    printf("%d %d\n", argc, optind);
    if (argc - optind != 3) return usage(argv[0]);
    inFolder = argv[optind];
    outFile = argv[optind + 1];
    outFolder = argv[optind + 2];

    printf("width: %u height: %u crash: %d infolder: %s outfile: %s outfolder: %s\n", width, height, crashOnBadFit, inFolder, outFile, outFolder);





    // TODO:
    // - 1 - find all of the image files in the given directory and add them to
    //       a list of some kind.
    // - 2 - sort that list so we have the longest ones first
    // - 3 - start adding them to a tree thingy that places them correctly. If
    //       there are any that are fundamentally too big, we can have a setting
    //       so it either ignores them or crashes. Also make the size of the
    //       overall image and whether or not to use padding a setting.
    // - 4 - 
    return 0;
}