/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN_GAME_SELECTVIEWBASE_HPP
#define SCREEN_GAME_SELECTVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen_game_select_screen/screen_game_selectPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>

class screen_game_selectViewBase : public touchgfx::View<screen_game_selectPresenter>
{
public:
    screen_game_selectViewBase();
    virtual ~screen_game_selectViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box bg_grey;
    touchgfx::Container btn_asteroids;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  _btn_asteroids;
    touchgfx::TextArea lbl_asteroids;
    touchgfx::Container btn_scoreboard;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  _btn_scoreboard;
    touchgfx::TextArea lbl_scoreboard;
    touchgfx::Container btn_logout;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  _btn_logout;
    touchgfx::Box background_light_grey;
    touchgfx::ScalableImage icon_lock;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<screen_game_selectViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // SCREEN_GAME_SELECTVIEWBASE_HPP
