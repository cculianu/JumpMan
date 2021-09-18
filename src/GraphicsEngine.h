/*!
 * \file GraphicsEngine.h
 * \brief File containing the GraphicsEngine class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 *
 * Heavily modified by Calin A. Culianu <calin.culianu@gmail.com>
 */
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <map>
#include <string>

using rect_t = SDL_Rect;

/*!
 * \enum text_color_t
 * \brief text colors available
 */
enum text_color_t {
    RED,     /*!< Red */
    GREEN,   /*!< Green */
    BLUE,    /*!< Blue */
    CYAN,    /*!< Cyan */
    MAGENTA, /*!< Magenta */
    YELLOW,  /*!< Yellow */
    ORANGE,  /*!< Orange */
    WHITE,   /*!< White */
    GRAY,    /*!< GRAY */
};

enum alignment_t { AlignLeft, AlignRight, AlignCenter };

/*!
 * \class GraphicsEngine
 * \brief Class for managing graphics and events
 *
 * GraphicsEngine handles all input from the user and
 * handles all drawing of images on the screen.
 * Only one GraphicsEngine can be active at one time
 */
class GraphicsEngine
{
public:
    /*!
     * \brief Contructor
     * \param title The title that will be seen on the titlebar
     * \param screen_width Size of game screen's width
     * \param screen_height Size of the game screen's height
     */
    GraphicsEngine(const std::string &title, const unsigned screen_width, const unsigned screen_height);

    /// Disabled copy constructor
    GraphicsEngine(const GraphicsEngine &) = delete;

    /// Destructor
    ~GraphicsEngine();

    /// Disabled copy constructor
    void operator=(const GraphicsEngine &) = delete;

    /*!
     * \brief Loads and image from the disk into the RAM
     * \param filename  Name of the image file inside graphics/ folder
     (example: to load graphics/player.png, filename should be "player")
     * \return true on success
     */
    bool loadImage(const std::string &filename);

    /*!
     * \brief returns a string describing last error that occurred
     */
    std::string getLastError() const;

    /*!
     * \brief fills screen width black paint
     */
    void makeScreenBlack();

    /*!
     * \brief Draw an image to the game screen
     * \param image filename of image to draw
     * \param srcrect rectangle of image to draw from
     * \param dstrect part to screen to draw to
     * \return true on success
     */
    bool drawImage(const std::string &image, rect_t *srcrect, rect_t *dstrect);

    /*!
     * \brief Draw some text at the given location
     * \param text text to draw
     * \param y the center on the y-axis where we will draw
     */
    void drawText(const std::string &text, unsigned y, text_color_t = CYAN, alignment_t = AlignCenter,
                  bool small = false, bool bright = false);

    /*!
     * \brief Swap backbuffer to screen
     * \return true on success
     */
    bool updateScreen();

    /// Returns width of game screen
    unsigned screen_width() const;

    /// Returns height of game screen
    unsigned screen_height() const;

private:
    /// Title to display on the game's status bar
    const std::string TITLE;

    /// Width of the game screen
    const unsigned SCREEN_WIDTH;

    /// Height of the game screen
    const unsigned SCREEN_HEIGHT;

    /// Map of filename and image we have loaded from disk
    std::map<std::string, SDL_Surface *> images_;

    /// Font to use
    TTF_Font *font_{}, *font_small_{};

    /// The game screen
    SDL_Window *win{};
    SDL_Surface *screen_{}; // the window's surface
};
