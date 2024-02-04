#ifndef SCREEN_ASTEROIDSPRESENTER_HPP
#define SCREEN_ASTEROIDSPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

#include "ship_direction.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

#define SHIP_MOVES_PER_TICK 5
#define ROCK_MOVES_PER_TICK 1
#define BULLET_MOVES_PER_TICK 10

#define ROCK_OFFSCREEN_X 0
#define ROCK_OFFSCREEN_Y 0

#define BULLET_OFFSCREEN_X 0
#define BULLET_OFFSCREEN_Y 480


using namespace touchgfx;

class screen_asteroidsView;

class screen_asteroidsPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    screen_asteroidsPresenter(screen_asteroidsView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~screen_asteroidsPresenter() {}

    /*
     * GENERAL GAME STUFF
     */
    virtual void resume_asteroids_task();
    virtual void suspend_asteroids_task();

    virtual bool is_asteroids_game_started();

    virtual void start_asteroids_game();
    virtual void init_game();
    virtual void game_over();

    virtual bool elements_collide(Drawable *element1, Drawable *element2);

    virtual void shoot_bullet();

    virtual void set_allowed_to_shoot(bool value);
    virtual bool is_allowed_to_shoot();

    virtual void set_invincible(bool value);
    virtual bool is_invincible();

    /*
     * SHIP STUFF
     */
    virtual void move_ship();
    virtual void check_ship_collisions();


    /*
     * ROCK STUFF
     */
    virtual void move_rocks();
    virtual void hide_rocks();


    /*
     * BULLET STUFF
     */
    virtual void move_bullets();
    virtual void hide_bullets();

    virtual Drawable* get_next_invisible_bullet();

    virtual void check_bullet_collisions();


private:
    screen_asteroidsPresenter();

    char lives = 3;
    unsigned int score = 0;
    unsigned int high_score = 0;

    ShipDirection direction = NONE;

    bool game_started = false;

    bool allowed_to_shoot = true;

    bool invincible = false;

    screen_asteroidsView& view;
};

#endif // SCREEN_ASTEROIDSPRESENTER_HPP
