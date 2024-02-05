#ifndef SCREEN_LOCKEDVIEW_HPP
#define SCREEN_LOCKEDVIEW_HPP

#include <gui_generated/screen_locked_screen/screen_lockedViewBase.hpp>
#include <gui/screen_locked_screen/screen_lockedPresenter.hpp>

class screen_lockedView : public screen_lockedViewBase
{
public:
    screen_lockedView();
    virtual ~screen_lockedView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN_LOCKEDVIEW_HPP
