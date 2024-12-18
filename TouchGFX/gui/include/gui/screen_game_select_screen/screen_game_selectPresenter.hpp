#ifndef SCREEN_GAME_SELECTPRESENTER_HPP
#define SCREEN_GAME_SELECTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class screen_game_selectView;

class screen_game_selectPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    screen_game_selectPresenter(screen_game_selectView& v);

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

    virtual ~screen_game_selectPresenter() {}

private:
    screen_game_selectPresenter();

    screen_game_selectView& view;
};

#endif // SCREEN_GAME_SELECTPRESENTER_HPP
