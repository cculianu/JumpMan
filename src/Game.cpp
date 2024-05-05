/*!
 * \file Game.cpp
 * \brief File containing the Game class source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 *
 * Heavily modified by Calin A. Culianu <calin.culianu@gmail.com>
 */
#include "Game.h"

#include "AudioEngine.h"
#include "BasicStar.h"
#include "GraphicsEngine.h"
#include "Highscore.h"
#include "MovingStar.h"
#include "tinyformat.h"

#include <cassert>
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
inline constexpr unsigned JETPACK_SOUND_DURATION_MS = 388;

struct Game::GameOver
{
    enum State { Begin, InputHS, PressAnyKey };
    State state = Begin;
    std::string nick{};
    size_t new_idx{};
    Highscore highscore;

    GameOver(const std::string &filename, std::function<void()> on_save = {}) : highscore{filename, on_save} {}
};


Game::Game()
{
    /* Initialize graphics */
    if constexpr (IS_IOS) {
        graphics_ = std::make_unique<GraphicsEngine>("Jumpman" /* Title */, 375 /* Screen width */, 667 /* Screen height */);
    } else {
        graphics_ = std::make_unique<GraphicsEngine>("Jumpman" /* Title */, 1000 /* Screen width */, 600 /* Screen height */);
    }

    /* Load images from disk */
    if (graphics_->loadImage("player") == false || graphics_->loadImage("basic_star") == false ||
        graphics_->loadImage("moving_star") == false) {
        FatalError(graphics_->getLastError(), "Failed to Load Image");
    }

    /* Initialize audio */
    audio_ = std::make_unique<AudioEngine>();
    audio_->loadBackgroundMusic("audio/ambient1.ogg");
    audio_->loadJetpackSoundEffect("audio/jetpack1.wav");
    audio_->loadStarSoundEffect("audio/starsound1.wav", "audio/starsound2.wav");
    audio_->startPlayingBackgroundMusic(50);

    player_ = std::make_unique<Player>(graphics_->screen_width());

    // If we are running under emscripten, set up the /data mountpoint
#ifdef __EMSCRIPTEN__
    EM_ASM(
        // Make a directory other than '/'
        FS.mkdir('/persistent_data');
        // Then mount with IDBFS type
        FS.mount(IDBFS, {}, '/persistent_data');
        // Then sync
        FS.syncfs(true, function (err) {});
    );
#endif
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

auto Game::runStep() -> RunStepResult
{
    using R = RunStepResult;

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

    if (!game_over) {
        /* Normal gameplay */

        if (handlePlayerInput())
            return R::Quit; // user quit

        if (!paused_) {
            if (letObjectsInteract(tdiff / PHYSICS_RATE) == 1) {
                // indicates game over if this is set
#ifdef __EMSCRIPTEN__
                game_over = std::make_unique<GameOver>("/persistent_data/highscore", []{
                    // sync
                    EM_ASM(FS.syncfs(false, function (err) {}););
                });
#else
                game_over = std::make_unique<GameOver>(".highscore");
#endif
            }
        }
    }

    drawObjectsToScreen();

    if (game_over) {
        /* Draw game over screen */
        if (const auto r = drawGameOverScreen(); r != R::Continue)
            return r; // restart, quit, or error
    }

    /* Flush backbuffer to screen */
    if (!graphics_->updateScreen())
        return R::Error; // graphics failure

    return R::Continue;
}

void Game::reset()
{
    /* Reset Player */
    player_->reset();

    /* Reset Starlist */
    star_list_.clear();

    game_over.reset();

    ticks_last_ = start_ticks_ = SDL_GetTicks();
}

int Game::run()
{
    using R = RunStepResult;
    reset();
    if constexpr (!IS_EMSCRIPTEN) {
        // Regular desktop app main loop
        R retval;
        do {
            // Advance game forward by 1 frame
            retval = runStep();

            if (retval == R::Restart) // retval == 2 indicates game restart
                reset();
        } while (retval == R::Continue || retval == R::Restart);
        return retval == R::Error ? 1 : 0;
    } else {
        // EMSCRIPTEN, use the weird callback mechanism to continually pass control to JS and not hang browser.
        try {
            emscripten_set_main_loop_arg([](void *arg){
                Game * const self = static_cast<Game *>(arg);
                switch (const R r = self->runStep()) {
                case R::Restart: // game restart
                    self->reset();
                    break;
                case R::Continue: // continue as normal
                    break;
                default:
                    // otherwise game exit (doesn't seem to work in emscripten :/)
                    std::_Exit(r == R::Error ? 1 : 0);
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
    while (auto optEvent = getEvent()) {
        event = *optEvent;
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
                audio_->playJetpackSound(); // only play sound if jumping did occur
            break;
        case PAUSEPLAY:
            paused_ = audio_->togglePausePlayBackgroundMusic();
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
                if (ok && audio_->lastPlayedJetpackSoundAgeMS() > JETPACK_SOUND_DURATION_MS)
                    audio_->playJetpackSound();
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

void Game::drawObjectsToScreen()
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

auto Game::drawGameOverScreen() -> RunStepResult
{
    assert(bool(game_over));

    using ST = Game::GameOver::State;
    using R = RunStepResult;
    auto & state = game_over->state;
    auto & highscore = game_over->highscore;
    size_t & new_idx = game_over->new_idx;
    std::string & nick = game_over->nick;

    if (state == ST::Begin) {
        if (highscore.add(player_->score(), &new_idx)) {
            // new high score
            state = ST::InputHS;
        } else {
            // not new high score
            state = ST::PressAnyKey;
        }
    }

    const signed screen_height = graphics_->screen_height();

    /* Header */
    graphics_->drawText("Highscore", 250);

    /* Draw every score from highscore */
    bool want_highlight = state == ST::InputHS;
    for (size_t i = 0, y = 300; i < game_over->highscore.size(); ++i) {
        text_color_t text_color = YELLOW;
        auto [score, name] = highscore.get(i);

        /* If we managed to get into the highscore, write the score in orange */
        if (want_highlight && i == new_idx) {
            text_color = ORANGE;
            want_highlight = false;
            graphics_->drawText("New highscore!", screen_height + 150);
            if (!nick.empty())
                name = nick; // overwrite with current user inputted nickname in high scores
        }
        if (score != 0 && !name.empty()) {
            if (name.size() < 5) name.resize(5, ' ');
            const std::string text = strprintf("%s  %7i", name, score);
            graphics_->drawText(text, y, text_color);
            y += 40;
        }
    }

    if (state == ST::InputHS) {
        /* If we managed to get into the highscore: Ask for nickname */
        graphics_->drawText("Enter your name (1-5 letters) and press enter", screen_height + 200);
        graphics_->drawText(nick.empty() ? " " : nick, screen_height + 250, ORANGE);
    } else if (state == ST::PressAnyKey) {
        /* Draw message that tells player that the game is over */
        graphics_->drawText("Press any key to continue", screen_height + 440);

    }

    // Drain event queue looking for keyboard or quit events
    while (const auto optPair = getKeyEvent()) {
        auto & [key, quit] = *optPair;
        if (quit) return R::Quit; // indicate user quit
        if (!key) continue; // was not a key event, keep processing events
        if (state == ST::InputHS) {
            if (key >= SDLK_a && key <= SDLK_z && nick.size() < 5)
                nick += static_cast<char>('A' + key - SDLK_a);
            else if (key == SDLK_RETURN && !nick.empty()) {
                highscore.setNickname(nick, new_idx); // save user nickname
                state = ST::PressAnyKey; // advance state
                break; // break out of while loop
            }
            else if (key == SDLK_BACKSPACE && !nick.empty())
                nick.resize(nick.size() - 1);
        } else if (state == ST::PressAnyKey) {
            // they pressed a key, indicate restart
            return R::Restart;
        }
    }

    return R::Continue;
}

std::optional<std::pair<int, bool>> Game::getKeyEvent() const
{
    std::optional<std::pair<int, bool>> ret;
    SDL_Event e;

    if (SDL_PollEvent(&e)) {
        /* If user presses the X in the upper right corner: quit */
        if (e.type == SDL_QUIT) {
            ret.emplace(0, true);
        } else if (e.type == SDL_KEYDOWN) {
            ret.emplace(e.key.keysym.sym, false);
        }
    }
    return ret;
}

auto Game::getEvent() const -> std::optional<event_t>
{
    std::optional<event_t> ret;
    SDL_Event sdl_event;

    // no events available
    if (SDL_PollEvent(&sdl_event) == 0)
        return ret;

    /* If user presses the X in the upper right corner: quit */
    if (sdl_event.type == SDL_QUIT)
        ret = QUIT;

    /* User presses a relevant key, return info */
    else if (sdl_event.type == SDL_KEYDOWN)
        switch (sdl_event.key.keysym.sym) {
        case SDLK_q:
            ret = QUIT;
            break;
        case SDLK_p:
            ret = PAUSEPLAY;
            break;
        case SDLK_LEFT:
            ret = LEFT;
            break;
        case SDLK_RIGHT:
            ret = RIGHT;
            break;
        case SDLK_UP:
            ret = UP;
            break;
        case SDLK_f:
            ret = FPS_TOGGLE;
            break;
        default:
            ret = NOTHING;
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
                ret = RIGHT;
            else
                ret = STILL;
            break;

        case SDLK_RIGHT:
            if (state[SDL_SCANCODE_LEFT])
                ret = LEFT;
            else
                ret = STILL;
            break;

        default:
            ret = NOTHING;
            break;
        }
    }

    /* All other SDL_Events are set to NOTHING */
    else
        ret = NOTHING;

    if constexpr (IS_IOS) {
        if (ret == NOTHING) {
            if (bool will{}; ((will=sdl_event.type == SDL_APP_WILLENTERBACKGROUND) || sdl_event.type == SDL_APP_DIDENTERBACKGROUND)
                             && !paused_) {
                // handle iOS pause due to app going into BG
                ret = PAUSEPLAY;
                SDL_Log("App %s go into background, pausing.", will ? "will" : "did");
            }
            else if (sdl_event.type == SDL_APP_DIDENTERFOREGROUND && paused_) {
                // handle iOS unpause due to app going into FG
                ret = PAUSEPLAY;
                SDL_Log("App went into foreground, unpausing.");
            }
            else if (sdl_event.type == SDL_MOUSEMOTION) {
                // handle mouse (touch) motion on iOS
                constexpr int LRTHRESH = 3, UDTHRESH = 4;
                int dx, dy;
                Uint8 const state = SDL_GetRelativeMouseState(&dx, &dy);  /* get its location */
                if (state & SDL_BUTTON_LMASK) {     /* is the mouse (touch) down? */
                    SDL_LogVerbose(SDL_LOG_CATEGORY_INPUT, "Mouse: dx: %d, dy: %d", dx, dy);
                    bool const mostlyVertical = std::abs(dx) < std::abs(dy);
                    ret = lastMouseDir;
                    if (mostlyVertical) {
                        if (dy < 0 && std::abs(dy) >= UDTHRESH) ret = lastMouseDir = UP;
                    } else {
                        if (dx >= LRTHRESH) ret = lastMouseDir = RIGHT;
                        else if (dx < 0 && dx <= LRTHRESH) ret = lastMouseDir = LEFT;
                    }
                }
            } else if (sdl_event.type == SDL_MOUSEBUTTONUP) {
                ret = lastMouseDir = STILL;
            }
        }
    }

    return ret;
}
