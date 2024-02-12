#include "Menu.hpp"

Textbox::Textbox(char const *text) {
    for (int i = 0; i < N_CHARS; i++) {
        buffer[i] = text[i];
        if (!text[i]) return;
    }
}

bool Textbox::update() {
    // TODO: simply wait until the user presses a key, and then gtfo. Maybe
    //       add a small delay so you can't exit too fast.
    return false;
}

void Textbox::render() const {
    // TODO: draw the text and the text box.
}