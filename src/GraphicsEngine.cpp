/*!
 * \file GraphicsEngine.cc
 * \brief File containing the GraphicsEngine source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */
#include "GraphicsEngine.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_image.h>

#include <algorithm>

GraphicsEngine::GraphicsEngine(const std::string &title, const unsigned screen_width, const unsigned screen_height,
                               const unsigned screen_bpp, const unsigned frame_rate)
    : TITLE(title), SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height), SCREEN_BPP(screen_bpp),
      FRAME_RATE(frame_rate), images_(), time_of_last_refresh_(SDL_GetTicks())
{
    /* Init SDL*/
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        Game::FatalError(SDL_GetError(), "Failed to Initialize SDL");

    /* Create a main screen */
    win = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                           SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    if (!win)
        Game::FatalError(SDL_GetError(), "Failed to Create Window");

    screen_ = SDL_GetWindowSurface(win);
    if (!screen_)
        Game::FatalError(SDL_GetError(), "Failed to Get SDL Surface");

    /* Init TTF */
    if (TTF_Init() == -1)
        Game::FatalError(TTF_GetError(), "Failed to Initialize TTF");

    /* Load font from disk */
    font_ = TTF_OpenFont("graphics/font.ttf", 20);
    if (font_ == nullptr)
        Game::FatalError(TTF_GetError(), "Failed to Load Font");

    /* Initialize timer */
    if (SDL_InitSubSystem(SDL_INIT_TIMER) == -1)
        Game::FatalError(SDL_GetError(), "Failed to Initialize SDL Timer");
}

GraphicsEngine::~GraphicsEngine()
{
    /* Unload all images */
    for (auto image : this->images_) {
        SDL_FreeSurface(image.second);
        image.second = nullptr;
    }

    /* Unload font */
    TTF_CloseFont(font_);

    TTF_Quit();
    SDL_DestroyWindow(win);
    // no need to free window surface
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool GraphicsEngine::loadImage(const std::string &filename)
{
    /* Check if image is already loaded */
    if (this->images_.count(filename))
        return true;

    /* Load image from disk: */
    const std::string real_filename = "graphics/" + filename + ".png";
    SDL_Surface *scratch_surface = IMG_Load(real_filename.c_str());
    if (scratch_surface == nullptr)
        return false;

    /* Set transparency (White is transparent); */
    SDL_SetColorKey(scratch_surface, SDL_TRUE, SDL_MapRGB(scratch_surface->format, 255, 255, 255));

    /* Format image to optimize it */
    SDL_Surface *optimized_image = SDL_ConvertSurface(scratch_surface, screen_->format, 0);
    SDL_FreeSurface(scratch_surface);
    if (optimized_image == nullptr)
        return false;

    /* Add it to list of images */
    this->images_[filename] = optimized_image;
    return true;
}

std::string GraphicsEngine::getLastError() const { return SDL_GetError(); }

void GraphicsEngine::makeScreenBlack() { SDL_FillRect(this->screen_, nullptr, 0); }

bool GraphicsEngine::drawImage(const std::string &filename, rect_t *srcrect, rect_t *dstrect)
{
    /* First, check if the image is loaded */
    SDL_Surface *image_to_blit = nullptr;
    if (this->images_.count(filename))
        image_to_blit = this->images_[filename];

    /* If image was not found, try to load it */
    if (image_to_blit == nullptr) {
        if (this->loadImage(filename) == false)
            return false;

        image_to_blit = this->images_[filename];
    }

    /* Doing some switcheroo here,
     * X:0 is now at the center of the screen
     * Y:0 is not at the bottom of the screen */
    if (dstrect != nullptr) {
        dstrect->x = SCREEN_WIDTH / 2 - dstrect->w / 2 + dstrect->x;
        dstrect->y = SCREEN_HEIGHT - dstrect->h - dstrect->y;
    }

    /* Draw it */
    SDL_BlitSurface(image_to_blit, srcrect, this->screen_, dstrect);

    return true;
}

void GraphicsEngine::drawText(const std::string &text, unsigned y, text_color_t text_color_name)
{
    SDL_Color text_color;
    const SDL_Color background_color = {0, 0, 0, 0};

    switch (text_color_name) {
    case CYAN:
        text_color = {0, 127, 127, 0};
        break;
    case YELLOW:
        text_color = {255, 255, 0, 0};
        break;
    case ORANGE:
        text_color = {255, 127, 0, 0};
        break;
    }

    /* Craete text */
    SDL_Surface *text_surface = TTF_RenderText_Shaded(this->font_, text.c_str(), text_color, background_color);

    /* Set transparency */
    SDL_SetColorKey(text_surface, SDL_TRUE, SDL_MapRGB(text_surface->format, 0, 0, 0));

    /* Set target rect */
    SDL_Rect dstrect = {static_cast<short>(SCREEN_WIDTH / 2 - text_surface->w / 2),
                        static_cast<short>(y / 2 - text_surface->h / 2), 0, 0};

    /* Blit and release */
    SDL_BlitSurface(text_surface, nullptr, this->screen_, &dstrect);
    SDL_FreeSurface(text_surface);
}

bool GraphicsEngine::updateScreen()
{
    const size_t TIME_SINCE_LAST_REFRESH = SDL_GetTicks() - this->time_of_last_refresh_;
    const size_t REFRESH_RATE = 1000 / FRAME_RATE;

    /* If the last refresh was too recent, we'll wait a while */
    if (TIME_SINCE_LAST_REFRESH < REFRESH_RATE)
        SDL_Delay(REFRESH_RATE - TIME_SINCE_LAST_REFRESH);

    /* Reset update-time and flush screen */
    this->time_of_last_refresh_ = SDL_GetTicks();
    return SDL_UpdateWindowSurface(win) == 0;
}

bool GraphicsEngine::getEvent(event_t &event) const
{
    SDL_Event sdl_event;
    bool status = SDL_PollEvent(&sdl_event);

    /* If user presses the X in the upper right corner: quit */
    if (sdl_event.type == SDL_QUIT)
        event = QUIT;

    /* User presses a relevant key, return info */
    else if (sdl_event.type == SDL_KEYDOWN)
        switch (sdl_event.key.keysym.sym) {
        case SDLK_q:
            event = QUIT;
            break;
        case SDLK_p:
            event = PAUSEPLAY;
            break;
        case SDLK_LEFT:
            event = LEFT;
            break;
        case SDLK_RIGHT:
            event = RIGHT;
            break;
        case SDLK_UP:
            event = UP;
            break;
        default:
            event = NOTHING;
            break;
        }

    /* If user releases
     * 1. Right arrow key but still holds the left: event = LEFT
     * 2. Left arrow key but still hold the right: event = RIGHT
     * 3. Any arrow key and does not hold any arrow key: event = STILL
     */
    else if (sdl_event.type == SDL_KEYUP) {
        const auto *state = SDL_GetKeyboardState(nullptr);
        switch (sdl_event.key.keysym.sym) {
        case SDLK_LEFT:
            if (state[SDL_SCANCODE_RIGHT])
                event = RIGHT;
            else
                event = STILL;
            break;

        case SDLK_RIGHT:
            if (state[SDL_SCANCODE_LEFT])
                event = LEFT;
            else
                event = STILL;
            break;

        default:
            event = NOTHING;
            break;
        }
    }

    /* All other SDL_Events are set to NOTHING */
    else
        event = NOTHING;

    return status;
}

void GraphicsEngine::getStringFromPlayer(unsigned max_letters, std::string &target, unsigned y)
{
    /* Make a backup of the part of the screen where we will write */
    SDL_Surface *scratch_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 100, 20, 32, 0, 0, 0, 0);
    SDL_Rect scrrect = {static_cast<short>(SCREEN_WIDTH / 2 - scratch_surface->w / 2),
                        static_cast<short>(y / 2 - scratch_surface->h / 2), 100, 20};
    SDL_BlitSurface(screen_, &scrrect, scratch_surface, nullptr);

    target.clear();
    SDL_Event event;
    for (;;) {
        /* Clear the screen and draw the string */
        SDL_BlitSurface(scratch_surface, nullptr, screen_, &scrrect);
        this->drawText(target.size() ? target : " ", y, ORANGE);
        SDL_UpdateWindowSurface(win);

        SDL_WaitEvent(&event);
        if (event.type == SDL_KEYDOWN) {
            /* If player types A-Z, add it to string */
            if (SDLK_a <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_q && target.size() < max_letters)
                target += static_cast<char>('A' + event.key.keysym.sym - SDLK_a);
            /* If player presses enter - return */
            else if (event.key.keysym.sym == SDLK_RETURN && target.size() > 0)
                break;
            /* If player pressed backspace - remove letter */
            else if (event.key.keysym.sym == SDLK_BACKSPACE && target.size() > 0)
                target = target.substr(0, target.size() - 1);
        }
    }

    SDL_FreeSurface(scratch_surface);
}

void GraphicsEngine::waitForKeypress() const
{
    SDL_Event event;
    for (;;) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_KEYDOWN)
            break;
    }
}

unsigned GraphicsEngine::screen_width() const { return this->SCREEN_WIDTH; }

unsigned GraphicsEngine::screen_height() const { return this->SCREEN_HEIGHT; }
