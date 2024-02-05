#ifndef SCREEN_SCOREBOARDVIEW_HPP
#define SCREEN_SCOREBOARDVIEW_HPP

#include <gui_generated/screen_scoreboard_screen/screen_scoreboardViewBase.hpp>
#include <gui/screen_scoreboard_screen/screen_scoreboardPresenter.hpp>

class screen_scoreboardView : public screen_scoreboardViewBase
{
public:
    screen_scoreboardView();
    virtual ~screen_scoreboardView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN_SCOREBOARDVIEW_HPP
