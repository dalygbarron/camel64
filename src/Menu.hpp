#pragma once

/**
 * Parent class of a menu that can be displayed over the top of whatever else is
 * going on. We will not implement a whole scene stack in this game, we will
 * just have normal scenes that can be switched between, and then these menus
 * which are placed on top of that. If we want to make something happen after a
 * menu has been returned from we can implement some kind of global message
 * queue that the menu can push stuff to before it closes.
 */
class Menu {
    public:
        /**
         * Allows the menu to update.
         * @return true if the menu wishes to continue next frame and false if
         *         it is ready to close.
         */
        virtual bool update() = 0;

        /**
         * Renders the menu to the screen.
         */
        virtual void render() const = 0;
};

/**
 * Displays a nice box with some text until the user presses some button to
 * dismiss it.
 */
class Textbox: public Menu {
    private:
        static int const N_CHARS = 255;
        char buffer[N_CHARS];

    public:
        /**
         * Just passes in the text that the textbox needs to show.
         * @param text is the text that the textbox will show. This text will be
         *        copied into a buffer, so after this constructor is called you
         *        had best delete it again or whatever you need to do.
         */
        Textbox(char const *text);

        virtual bool update() override;

        virtual void render() const override;
};