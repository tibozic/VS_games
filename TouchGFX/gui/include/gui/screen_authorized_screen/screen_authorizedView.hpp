#ifndef SCREEN_AUTHORIZEDVIEW_HPP
#define SCREEN_AUTHORIZEDVIEW_HPP

#include <gui_generated/screen_authorized_screen/screen_authorizedViewBase.hpp>
#include <gui/screen_authorized_screen/screen_authorizedPresenter.hpp>

class screen_authorizedView : public screen_authorizedViewBase
{
public:
    screen_authorizedView();
    virtual ~screen_authorizedView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void set_player_data(char *name, int player_id);
protected:
};

#endif // SCREEN_AUTHORIZEDVIEW_HPP
