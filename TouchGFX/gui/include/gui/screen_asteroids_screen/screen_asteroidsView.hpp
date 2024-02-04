#ifndef SCREEN_ASTEROIDSVIEW_HPP
#define SCREEN_ASTEROIDSVIEW_HPP

#include <gui_generated/screen_asteroids_screen/screen_asteroidsViewBase.hpp>
#include <gui/screen_asteroids_screen/screen_asteroidsPresenter.hpp>

#include <stdlib.h>

#include <images/BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

#include "ship_direction.h"

#define ROCK_COUNT 10
#define BULLET_COUNT 10

class screen_asteroidsView : public screen_asteroidsViewBase
{
public:
    touchgfx::ScalableImage rocks[ROCK_COUNT];
    touchgfx::Box bullets[BULLET_COUNT];

    screen_asteroidsView();
    virtual ~screen_asteroidsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void resume_asteroids_task();
    virtual void suspend_asteroids_task();

    /*
     * GENERAL GAME STUFF
     * */
    virtual void start_asteroids_game();

    virtual void update_hearts(char lives);
    virtual void update_score(unsigned int score);

    virtual void show_endgame_screen();
    virtual void hide_endgame_screen();

    virtual void hide_element(Drawable *element);

    /*
     * SHIP STUFF
     * */
    virtual void getShipPosition(int *x, int *y);
    virtual void move_ship(int x, int y);

    void move_rock_offscreen(touchgfx::ScalableImage *rock);

    virtual Drawable* get_ship_pointer();
    virtual ShipDirection get_ship_direction();

    /*
     * ROCK STUFF
     * */
    virtual void move_rock(touchgfx::ScalableImage *rock, int x, int y);
    virtual void new_rock_position(touchgfx::ScalableImage *rock);

    /*
     * BULLET STUFF
     */
    virtual bool bullet_button_pressed();
    virtual void move_bullet_offscreen(touchgfx::Box *bullet);
    virtual void move_bullet(touchgfx::Box *bullet, int x, int y);

protected:
};

#endif // SCREEN_ASTEROIDSVIEW_HPP
