#ifndef MODEL_HPP
#define MODEL_HPP

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
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
