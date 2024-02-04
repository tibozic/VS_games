#ifndef SCREEN_GAME_SELECTVIEW_HPP
#define SCREEN_GAME_SELECTVIEW_HPP

#include <gui_generated/screen_game_select_screen/screen_game_selectViewBase.hpp>
#include <gui/screen_game_select_screen/screen_game_selectPresenter.hpp>

class screen_game_selectView : public screen_game_selectViewBase
{
public:
    screen_game_selectView();
    virtual ~screen_game_selectView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN_GAME_SELECTVIEW_HPP
