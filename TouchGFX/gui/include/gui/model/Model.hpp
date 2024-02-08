#ifndef MODEL_HPP
#define MODEL_HPP

#include "score_entry.h"

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


    score_entry scores[5];

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
