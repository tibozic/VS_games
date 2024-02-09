#ifndef MODEL_HPP
#define MODEL_HPP

#include <string.h>

#include "score_entry.h"
#include <stdint.h>



class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void resume_asteroids_task();
    void suspend_asteroids_task();

    void asteroids_tick();

    void start_bullet_timer();
    void bullet_timer_ended();

    void start_invincibility_timer();
    void invincibility_timer_ended();

    void update_scoreboard();

    void login(uint32_t tag_id);
    char* get_player_name();
    int get_player_id();
    void check_tag_card_scan();

    score_entry scores[5];

protected:
    char player_name[20] = "Guest";
    int player_id = 0;

    ModelListener* modelListener;
};

#endif // MODEL_HPP
