#pragma once

#include <stdint.h>
#include <GL/gl.h>
#include "glm/gtc/type_ptr.hpp"

/**
 * This is where the utilities are sitting around.
 */
namespace Util {
    extern color_t WHITE;

    enum Font: uint8_t {
        BODY = 1,
        HEADING = 2
    };

    /**
     * Loads in all the fonts that we want, so you should probably call this
     * before you use those font enums etc.
     */
    void initFonts();

    /**
     * Just a rectangle lol.
     */
    class Rect {
        public:
            glm::vec2 pos;
            glm::vec2 size;

            /**
             * Creates a rect with 0 in all fields.
             */
            Rect();

            /**
             * Sets the values of the rect directly.
             * @param pos is the position of the rect.
             * @param size is the size of the rect.
             */
            Rect(glm::vec2 pos, glm::vec2 size);

            /**
             * Sets each component of the rect individually.
             * @param x is pos.x.
             * @param y is pos.y.
             * @param w is size.x.
             * @param h is size.y.
             */
            Rect(float x, float y, float w, float h);

            /**
             * Gives you the position of the bottom right corner of the rect.
             * @return right corner.
             */
            glm::vec2 r() const;

            /**
             * Gives you the position of the top right corner of the rect.
             * @return top right corner.
             */
            glm::vec2 tr() const;

            /**
             * Gives you the position of the top left corner of the rect.
             * @return top left corner.
             */
            glm::vec2 tl() const;
    };

    /**
     * Takes string and compares it to a whole bunch of strings and finds which
     * of these strings if any it is equal to. Basically this just exists
     * because doing strcmp against a bunch of strings seperately is a waste of
     * time.
     * @param string is the string that is compared to all others.
     * @param ... is all the other strings. You need to provide at least one or
     *        it will crash.
     * @return the index of the string that the test string is equal to, or -1
     *         if it is not equal to any of them.
     */
    int selectString(char const *string, ...);

    /**
     * Generates a crappy hash of a string.
     */
    unsigned int hashString(char const *string);

    /**
     * Linked list with names. Only really for use in hashmap.
     */
    template <class T> class Bucket {
        public:
            char const *key = NULL;
            Bucket<T> *next = NULL;
            T data;
    };

    /**
     * Yep you can associate a string to whatever thingy your heart may desire
     * and it will find it in something along the lines of constant time in the
     * best case.
     */
    template <class T> class Hashmap {
        private:
            unsigned int nBuckets;
            Bucket<T> *buckets;

            /**
             * Finds the index of which bucket a given key should fit in.
             * @param key is the key to look up.
             * @return the bucket index.
             */
            inline unsigned int index(char const *key) {
                return hashString(key) % nBuckets;
            }

        public:
            /**
             * Creates the hashmap.
             * @param nBuckets the number of buckets that this hashmap can have.
             *        The more buckets the less chance of hash collisions which
             *        screw the performance.
             */
            Hashmap(unsigned int nBuckets) {
                this->nBuckets = nBuckets;
                buckets = new Bucket<T>[nBuckets];
            }

            /**
             * Adds an item to the hashmap. If you add a key that already exists
             * then you will be rewarded with undefined behaviour.
             * @param key is the key to save the thing under.
             * @param data is the thing to add.
             */
            void add(char const *key, T data) {
                Bucket<T> *bucket = buckets + index(key);
                while (true) {
                    if (!bucket->key) {
                        bucket->key = key;
                        bucket->data = data;
                        return;
                    } else if (!bucket->next) {
                        bucket->next = new Bucket<T>();
                    }
                    bucket = bucket->next;
                }
            }

            /**
             * Removes the item with this key. (Like I mentioned in add, if you
             * have added multiple things with the same key that is undefined
             * behavior and I can't guarantee remove will remove them all). If
             * nothing with that key can be found it just does nothing and
             * returns silently. Also, there will be no delete calls or
             * anything like that. If you put pointers into the hashmap then
             * it's your responsibility to delete them.
             * @param key is the key of the item to remove.
             */
            void remove(char const *key) {
                Bucket<T> *bucket = buckets + index(key);
                while (bucket) {
                    if (bucket->key && strcmp(bucket->key, key) == 0) {
                        return &bucket->data;
                    }
                    bucket = bucket->next;
                }
            }

            /**
             * Gets the element from the map with the given key if it exists.
             * @param key is the key under which to find the item.
             * @return a pointer to the found item if it could be found, or
             *         otherwise null.
             */
            T *find(char const *key) {
                Bucket<T> *bucket = buckets + index(key);
                while (bucket) {
                    if (bucket->key && strcmp(bucket->key, key) == 0) {
                        return &bucket->data;
                    }
                    bucket = bucket->next;
                }
                return NULL;
            }

            /**
             * Tells you if the given key exists in the hashmap.
             * @param key is the key to look for.
             * @return true if there is something with this key otherwise false.
             */
            bool has(char const *key) {
                Bucket<T> *bucket = buckets + index(key);
                while (bucket) {
                    if (bucket->key && strcmp(bucket->key, key) == 0) {
                        return true;
                    }
                    bucket = bucket->next;
                }
                return false;
            }
    };

    /**
     * Tells you if the given character is whitespace.
     * @param c is the character to check.
     * @return true iff it's whitespace.
     */
    bool whitespace(char c);

    /**
     * Wraps around a file and lets you read out tokens from it one at a time.
     */
    class TokenReader {
        public:
            static int const BUFFER_SIZE = 64;

        private:
            FILE *input;
            char inputBuffer[BUFFER_SIZE];
            char outputBuffer[BUFFER_SIZE];
            char const *cursor;
            char *outputCursor;

            /**
             * Fills the buffer
            */
            void refresh();

        public:
            /**
             * Creates the reader.
             * @param filename is the file that it is going to read tokens from.
             */
            TokenReader(char const *filename);

            /**
             * Closes the file handle.
             */
            ~TokenReader();

            /**
             * Gets the next token. A token is a sequence of characters that has
             * got whitespace or file start/end on either side of it. Also if
             * there is a token that is more than BUFFER_SIZE - 1 characters big
             * it will be broken into multiple tokens.
             * @return the next token or null if it's finished. The token
             *         pointer will be valid until you next call next or the
             *         TokenReader is destroyed so if you need it longer than
             *         that you had better save it.
             */
            char const *next();

            /**
             * Reads in the next token, which MUST exist, and parses it into an
             * integer. No idea what happens if it's not actually an integer
             * but probably something freaky.
             * @return the integer it parsed.
             */
            int nextInt();

            /**
             * Tells you if there is more to be read.
             * @return true iff the next time you call next it won't return
             *         null.
             */
            bool hasMore();
    };
};