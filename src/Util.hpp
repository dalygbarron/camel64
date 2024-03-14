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