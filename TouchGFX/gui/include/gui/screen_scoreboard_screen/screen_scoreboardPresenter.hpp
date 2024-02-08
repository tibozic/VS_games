#ifndef SCREEN_SCOREBOARDPRESENTER_HPP
#define SCREEN_SCOREBOARDPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class screen_scoreboardView;

class screen_scoreboardPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    screen_scoreboardPresenter(screen_scoreboardView& v);

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

    virtual ~screen_scoreboardPresenter() {}

    virtual void update_scoreboard();

private:
    screen_scoreboardPresenter();

    screen_scoreboardView& view;
};

#endif // SCREEN_SCOREBOARDPRESENTER_HPP
