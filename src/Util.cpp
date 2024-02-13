#include "Util.hpp"
#include <libdragon.h>
#include <cstdarg>

using namespace Util;

color_t Util::WHITE = {255, 255, 255, 255};

void Util::initFonts() {
    rdpq_text_register_font(
        Font::BODY,
        rdpq_font_load("rom:/fonts/lato.font64")
    );
    rdpq_text_register_font(
        Font::HEADING,
        rdpq_font_load("rom:/fonts/english.font64")
    );
}

Rect::Rect(): pos(), size() {}

Rect::Rect(glm::vec2 pos, glm::vec2 size): pos(pos), size(size) {}

Rect::Rect(float x, float y, float w, float h): pos(x, y), size(w, h) {}

glm::vec2 Rect::r() const {
    return glm::vec2(pos.x + size.x, pos.y);
}

glm::vec2 Rect::tr() const {
    return pos + size;
}

glm::vec2 Rect::tl() const {
    return glm::vec2(pos.x, pos.y + size.y);
}

int selectString(char const *string, ...) {
    int n = 0;
    va_list args;
    va_start(args, string);
    return n;
}

unsigned int hashString(char const *string) {
    unsigned int hash = 0;
    for (int i = 0; string[i]; i++) {
        hash += string[i];
    }
    return hash;
}

bool Util::whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}

void TokenReader::refresh() {
    size_t n = fread(inputBuffer, sizeof(char), BUFFER_SIZE, input);
    if (n < BUFFER_SIZE) inputBuffer[n] = 0;
    cursor = inputBuffer;
}

TokenReader::TokenReader(char const *filename) {
    input = fopen(filename, "r");
    if (input) refresh();
    else fprintf(stderr, "Couldn't open file %s\n", filename);
}

TokenReader::~TokenReader() {
    if (input) fclose(input);
}

char const *TokenReader::next() {
    if (!input) return NULL;
    outputCursor = outputBuffer;
    while (true) {
        char c = *(cursor++);
        bool white = whitespace(c);
        if (cursor - inputBuffer == BUFFER_SIZE) refresh();
        if (c == 0 || (white && outputCursor != outputBuffer)) {
            bool empty = outputCursor == outputBuffer;
            *(outputCursor++) = 0;
            if (c == 0) {
                fclose(input);
                input = NULL;
            }
            return empty ? NULL : outputBuffer;
        } else if (!white) {
            *(outputCursor++) = c;
        }
        if (outputCursor - outputBuffer == BUFFER_SIZE - 2) {
            outputBuffer[BUFFER_SIZE - 1] = 0;
            return outputBuffer;
        }
    }
}

int TokenReader::nextInt() {
    char const *token = next();
    assert(token);
    return atoi(token);
}

bool TokenReader::hasMore() {
    return input;
}