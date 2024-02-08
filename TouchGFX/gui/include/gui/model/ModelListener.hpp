#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void move_ship(){}
    virtual void move_bullets(){}
    virtual void move_rocks(){}

    virtual bool is_asteroids_game_started() {return false;}

    virtual void check_ship_collisions(){}
    virtual void check_bullet_collisions(){}

    virtual void set_allowed_to_shoot(bool value){}

    virtual void set_invincible(bool value){}

    virtual void update_scoreboard(){}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
