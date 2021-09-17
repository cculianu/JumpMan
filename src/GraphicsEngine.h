#pragma once

/*! 
 * \file GraphicsEngine.hh
 * \brief File containing the GraphicsEngine class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */
#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <map>


typedef SDL_Rect rect_t;

/*!
 * \enum event_t
 * \brief events describing user input
 */
enum event_t 
{ 
  LEFT,      /*!< Player wants to move left */
  RIGHT,     /*!< Player wants to move right */
  UP,        /*!< Player wants to jump */
  STILL,     /*!< Player wants to stop moving */
  PAUSEPLAY, /*!< Player wants to pause/play music */
  NOTHING,   /*!< Unknown input received */
  QUIT       /*!< User wants to exit the game */
};

/*!
 * \enum text_color_t
 * \brief text colors available 
 */
enum text_color_t
{
  CYAN,    /*!< Cyan */
  YELLOW,  /*!< Yellow */
  ORANGE   /*!< Orange */
};

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
     * \param screen_bpp The amount of bits per pixel (color)
     * \param frame_rate The frames per second we want to display
     */
    GraphicsEngine(const std::string &title,
                   const unsigned screen_width,
                   const unsigned screen_height,
                   const unsigned screen_bpp,
                   const unsigned frame_rate);

    ///Disabled copy constructor
    GraphicsEngine(const GraphicsEngine&) = delete;

    ///Destructor
    ~GraphicsEngine();

    ///Disabled copy constructor
    void operator=(const GraphicsEngine&) = delete;

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
    void drawText(const std::string &text, unsigned y, text_color_t = CYAN);

    /*!
     * \brief Flushes the screen so it's visible to the user 
     * \return true on success
     */
    bool updateScreen();

    /*!
     * \brief Get a string from player
     * \param max_letters how many letters are the max
     * \param target string to store the received string
     * \param y the center on the y-axis where we will draw
     */
    void getStringFromPlayer(unsigned max_letters, std::string &target,
                             unsigned y);

    /*!
     * \brief Non-blocking function to check event depending on user input.
     * \param event The event received from user
     * \return true if there are more pending events 
     */
    bool getEvent(event_t &event) const;

    /// Waits until a key is pressed
    void waitForKeypress() const;

    ///Returns width of game screen
    unsigned screen_width() const;

    ///Returns height of game screen
    unsigned screen_height() const;


  private:

    ///Title to display on the game's status bar
    const std::string TITLE;

    ///Width of the game screen
    const unsigned SCREEN_WIDTH;

    ///Height of the game screen
    const unsigned SCREEN_HEIGHT;

    ///Screen bits per pixel (color)
    const unsigned SCREEN_BPP;

    ///Screen frames per second
    const unsigned short FRAME_RATE;

    ///Map of filename and image we have loaded from disk
    std::map<std::string, SDL_Surface *> images_;

    ///The time updateScreen() last was called
    size_t time_of_last_refresh_;

    ///Font to use
    TTF_Font *font_{};

    ///The game screen
    SDL_Window *win{};
    SDL_Surface *screen_{}; // the window's surface
};
