/*!
 * \file Game.cc
 * \brief File containing the Game class source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */
#include "Game.h"

#include "AudioEngine.h"
#include "BasicStar.h"
#include "GraphicsEngine.h"
#include "Highscore.h"
#include "MovingStar.h"

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
inline constexpr bool IS_EMSCRIPTEN = true;
#else
inline constexpr bool IS_EMSCRIPTEN = false;
// emulated function so we can use if constexpr below....
template <typename ...Args>
void emscripten_set_main_loop_arg(Args && ...args [[maybe_unused]]) {
    assert(!"emscripten_set_main_loop_arg called in non-EMSCRIPTEN code! FIXME!");
}
#endif // __EMSCRIPTEN__

inline constexpr unsigned FRAME_RATE = 60; /* desired game framerate (non-EMSCRIPTEN only) */
inline constexpr unsigned REFRESH_RATE = 1000 / FRAME_RATE;
inline constexpr double   PHYSICS_RATE = 1000.0 / 24.0; /* internal physics originally assumed this framerate */
inline constexpr unsigned JETPACK_SOUND_DURATION_MS = 388;

Game::Game()
{
    /* Initialize graphics */
    graphics_ = std::make_unique<GraphicsEngine>("Jumpman" /* Title */,
                                                 1000 /* Screen width */, 600 /* Screen height */,
                                                 32 /* Bits per pixel */);

    /* Load images from disk */
    if (graphics_->loadImage("player") == false || graphics_->loadImage("basic_star") == false ||
        graphics_->loadImage("moving_star") == false) {
        FatalError(graphics_->getLastError(), "Failed to Load Image");
    }

    /* Initialize audio */
    audio_ = std::make_unique<AudioEngine>();
    audio_->loadBackgroundMusic("audio/ambient1.mp3");
    audio_->loadJetPackSoundEffect("audio/jetpack1.wav");
    audio_->loadStarSoundEffect("audio/starsound1.wav", "audio/starsound2.wav");
    audio_->startPlayingBackgroundMusic(50);

    player_ = std::make_unique<Player>(graphics_->screen_width());
}

Game::~Game()
{
}

[[noreturn]] /* static */
void Game::FatalError(const std::string &errMsg, const std::string &title)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), errMsg.c_str(), nullptr);
    std::_Exit(1);
}

/* static */
void Game::Warning(const std::string &msg)
{
    std::cerr << "Warning: " << msg << "\n";
}

/// Get a random number generator for the range [a, b]
/* static */
Game::RandGen Game::GetRandGen(int from, int to)
{
    static std::random_device rd;  // Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    assert(to >= from);
    return RandGen(gen, from, to);
}

int Game::runStep()
{
    unsigned tdiff = SDL_GetTicks() - ticks_last_;

    // throttle game frame-rate (non-emscripten mode only)
    if constexpr (!IS_EMSCRIPTEN) {
        if (tdiff < REFRESH_RATE) {
            SDL_Delay(REFRESH_RATE - tdiff);
            tdiff = SDL_GetTicks() - ticks_last_;
        }
    }

    // take rolling average of last 10 values
    fps_ = fps_ * 10.0 + 1000.0 / tdiff;
    fps_ /= 11.0;

    ticks_last_ = SDL_GetTicks();

    if (handlePlayerInput())
        return 0; // user quit

    if (letObjectsInteract(tdiff / PHYSICS_RATE) == 1) {
        gameOver(); // TODO: fix this to not use a local event loop (hangs on emscripten)
        return 2;
    }

    if (!drawObjectsToScreen())
        return 1; // graphics failure

    return 3;
}

void Game::reset()
{
    /* Reset Player */
    player_->reset();

    /* Reset Starlist */
    star_list_.clear();

    ticks_last_ = start_ticks_ = SDL_GetTicks();
}

int Game::run()
{
    reset();
    if constexpr (!IS_EMSCRIPTEN) {
        // Regular desktop app main loop
        int retval;
        do {
            // Advance game forward by 1 frame
            retval = runStep();

            if (retval == 2) // retval == 2 indicates game restart
                reset();
        } while (retval == 2 || retval == 3);
        return retval;
    } else {
        // EMSCRIPTEN, use the weird callback mechanism to continually pass control to JS and not hang browser.
        try {
            emscripten_set_main_loop_arg([](void *arg){
                Game * const self = static_cast<Game *>(arg);
                switch (const int r = self->runStep()) {
                case 2: // game restart
                    self->reset();
                    break;
                case 3: // continue as normal
                    break;
                default:
                    // otherwise game exit (doesn't seem to work in emscripten :/)
                    std::_Exit(r);
                    break;
                }
            }, this, 0, 1);
        } catch (...) {}
        return 0;
    }
}

bool Game::handlePlayerInput()
{
    event_t event = NOTHING;
    while (graphics_->getEvent(event))
        switch (event) {
        case LEFT:
            player_->move(-1);
            break;
        case RIGHT:
            player_->move(1);
            break;
        case STILL:
            player_->move(0);
            break;
        case UP:
            if (player_->jump())
                audio_->playJetPackSound(); // only play sound if jumping did occur
            break;
        case PAUSEPLAY:
            audio_->togglePausePlayBackgroundMusic();
            break;
        case QUIT:
            return true;
            break;
        case FPS_TOGGLE:
            show_fps_ = !show_fps_;
            break;
        default:
            break;
        }
    return false;
}

int Game::letObjectsInteract(double dt)
{
    // takeAction handles gravity
    player_->takeAction(dt);
    for (auto & star : star_list_)
        star->takeAction(dt);

    /* Remove stars if the player touches them or they disappear off screen */
    for (auto it = star_list_.begin(); it != star_list_.end();)
        if (bool touches = player_->touches(it->get()); touches || (*it)->y() < 0) {
            if (touches) {
                bool const moving_star = bool(dynamic_cast<MovingStar *>(it->get()));
                bool const ok = player_->jump(1 + moving_star);
                if (ok && audio_->lastPlayedJetPackSoundAgeMS() > JETPACK_SOUND_DURATION_MS)
                    audio_->playJetPackSound();
                audio_->playStarSound();
                if (moving_star) audio_->playStarSound(true);
            }
            it = star_list_.erase(it);
        } else
            ++it;

    /* Add stars if there is room */
    addStars();

    /* If player falls below the screen - return game over */
    if (player_->y() < -player_->height() * 2)
        return 1;

    /* If player is above the middle of the screen,
     * lower everything to center the player */
    const int offset_y = player_->y() - graphics_->screen_height() / 2;
    if (offset_y > 0) {
        player_->modifyY(-offset_y);
        for (auto &star : star_list_)
            star->modifyY(-offset_y);
    }
    return 0;
}

bool Game::drawObjectsToScreen()
{
    rect_t draw_to;
    rect_t draw_from;

    /* Draw background black */
    graphics_->makeScreenBlack();

    /* Draw all stars */
    for (auto &star : star_list_) {
        draw_to = {star->x(), star->y(), star->width(), star->height()};
        draw_from = {star->imageX(), 0, draw_to.w, draw_to.h};
        graphics_->drawImage(star->filename(), &draw_from, &draw_to);
    }

    /* Draw player */
    draw_to = {player_->x(), player_->y(), player_->width(), player_->height()};
    draw_from = {player_->imageX(), player_->imageY(), player_->width(), player_->height()};
    graphics_->drawImage(player_->filename(), &draw_from, &draw_to);

    /* Draw score */
    const std::string score_string = "Score: " + std::to_string(player_->score());
    graphics_->drawText(score_string, 20);

    const std::string velocity_string = "Velocity: " + std::to_string(int(std::round(player_->velocity()))) + " m/s ";
    graphics_->drawText(velocity_string, 20, WHITE, AlignRight, true);

    /* Draw instructions after 5 seconds of no jumps */
    if (player_->isStandingOnFloor() && SDL_GetTicks() - start_ticks_ > 5000) {
        graphics_->drawText("UP to jump",
                            graphics_->screen_height() + 440, CYAN, AlignCenter);
    }

    /* Draw FPS */
    if (show_fps_) {
        graphics_->drawText(" FPS: " + std::to_string(int(std::round(fps_))),
                            graphics_->screen_height()*2 - 20,  GREEN, AlignLeft, true, true);
    }

    /* Flush */
    return graphics_->updateScreen();
}

void Game::addStars()
{
    const signed screen_height = static_cast<signed>(graphics_->screen_height());
    const signed half_screen_width = graphics_->screen_width() / 2;

    /* Make sure there's always at least one star in the starlist
     * This is just to avoid segfaults */
    if (star_list_.size() <= 0)
        star_list_.emplace_back(new BasicStar(0, half_screen_width));

    /* Make sure there's a BasicStar every 50 y-pixels,
     * Also add other types of stars if the RNG is with you */
    auto rgen = GetRandGen(0, 6);
    while (star_list_.back()->initialY() < screen_height) {
        const short last_y = star_list_.back()->initialY();

        star_list_.emplace_back(new BasicStar(last_y, half_screen_width));

        if (rgen() == 1)
            star_list_.emplace_back(new MovingStar(last_y, half_screen_width));
    }
}

void Game::gameOver()
{
    Highscore highscore(".highscore");
    bool new_highscore = highscore.add(player_->score());

    const signed screen_height = graphics_->screen_height();

    /* Header */
    graphics_->drawText("Highscore", 250);

    /* Draw every score from highscore */
    bool highlight_next = new_highscore;
    for (size_t i = 0; i < highscore.size(); ++i) {
        text_color_t text_color = YELLOW;
        std::string new_score = highscore.get(i);

        /* If we managed to get into the highscore, write the score in orange */
        if (highlight_next && player_->score() == stoul(new_score)) {
            text_color = ORANGE;
            highlight_next = false;
            graphics_->drawText("New highscore!", screen_height + 150);
        }
        graphics_->drawText(new_score, 300 + (i * 40), text_color);
    }

    /* If we managed to get into the highscore: Ask for nickname */
    if (new_highscore) {
        std::string nick;
        graphics_->drawText("Enter your name (1-5 letters) and press enter", screen_height + 200);
        graphics_->getStringFromPlayer(5, nick, screen_height + 250);
        highscore.setNickname(nick);
    }

    /* Draw message that tells player that the game is over */
    graphics_->drawText("Press any key to continue", screen_height + 440);
    graphics_->updateScreen();
    graphics_->waitForKeypress();
}
