/*!
 * \file GraphicsEngine.cpp
 * \brief File containing the GraphicsEngine source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 *
 * Heavily modified by Calin A. Culianu <calin.culianu@gmail.com>
 */
#include "GraphicsEngine.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_image.h>

#include <algorithm>

GraphicsEngine::GraphicsEngine(const std::string &title, const unsigned screen_width, const unsigned screen_height)
    : TITLE(title), SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height)
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
    font_small_ = TTF_OpenFont("graphics/font.ttf", 14);
    if (font_small_ == nullptr)
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
    TTF_CloseFont(font_small_);

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

void GraphicsEngine::drawText(const std::string &text, unsigned y, text_color_t text_color_name, alignment_t align,
                              bool small, bool bright)
{
    SDL_Color text_color;
    const SDL_Color background_color = {0, 0, 0, 0};

    switch (text_color_name) {
    case RED: text_color = {127, 0, 0, 0}; break;
    case GREEN: text_color = {0, 127, 0, 0}; break;
    case BLUE: text_color = {0, 0, 127, 0}; break;
    case CYAN: text_color = {0, 127, 127, 0}; break;
    case MAGENTA: text_color = {127, 0, 127, 0}; break;
    case YELLOW: text_color = {255, 255, 0, 0}; break;
    case ORANGE: text_color = {255, 127, 0, 0}; break;
    case WHITE: text_color = {255, 255, 255, 0}; break;
    case GRAY: text_color = {127, 127, 127, 0}; break;
    }

    if (bright) {
        text_color.r = std::max(text_color.r * 2, 16);
        text_color.g = std::max(text_color.g * 2, 16);
        text_color.b = std::max(text_color.b * 2, 16);
    }

    /* Craete text */
    SDL_Surface *text_surface = TTF_RenderText_Shaded(small ? font_small_ :font_,
                                                      text.c_str(), text_color, background_color);

    /* Set transparency */
    SDL_SetColorKey(text_surface, SDL_TRUE, SDL_MapRGB(text_surface->format, 0, 0, 0));

    int pos_x{};
    switch (align) {
    case AlignLeft: pos_x = 0; break;
    case AlignCenter: pos_x = SCREEN_WIDTH / 2 - text_surface->w / 2; break;
    case AlignRight: pos_x = SCREEN_WIDTH - text_surface->w;
    }

    /* Set target rect */
    SDL_Rect dstrect{pos_x, static_cast<int>(y) / 2 - text_surface->h / 2, 0, 0};

    /* Blit and release */
    SDL_BlitSurface(text_surface, nullptr, this->screen_, &dstrect);
    SDL_FreeSurface(text_surface);
}

bool GraphicsEngine::updateScreen()
{
    return SDL_UpdateWindowSurface(win) == 0;
}

unsigned GraphicsEngine::screen_width() const { return this->SCREEN_WIDTH; }

unsigned GraphicsEngine::screen_height() const { return this->SCREEN_HEIGHT; }
