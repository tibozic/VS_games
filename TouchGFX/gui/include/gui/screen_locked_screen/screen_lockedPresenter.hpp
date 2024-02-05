#ifndef SCREEN_LOCKEDPRESENTER_HPP
#define SCREEN_LOCKEDPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class screen_lockedView;

class screen_lockedPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    screen_lockedPresenter(screen_lockedView& v);

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

    virtual ~screen_lockedPresenter() {}

private:
    screen_lockedPresenter();

    screen_lockedView& view;
};

#endif // SCREEN_LOCKEDPRESENTER_HPP
