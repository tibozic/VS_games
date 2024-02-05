#ifndef SCREEN_AUTHORIZEDPRESENTER_HPP
#define SCREEN_AUTHORIZEDPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class screen_authorizedView;

class screen_authorizedPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    screen_authorizedPresenter(screen_authorizedView& v);

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

    virtual ~screen_authorizedPresenter() {}

private:
    screen_authorizedPresenter();

    screen_authorizedView& view;
};

#endif // SCREEN_AUTHORIZEDPRESENTER_HPP
