/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN_LOCKEDVIEWBASE_HPP
#define SCREEN_LOCKEDVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen_locked_screen/screen_lockedPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/widgets/TextArea.hpp>

class screen_lockedViewBase : public touchgfx::View<screen_lockedPresenter>
{
public:
    screen_lockedViewBase();
    virtual ~screen_lockedViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  btn_next;
    touchgfx::Box bg_red;
    touchgfx::ScalableImage icon_lock;
    touchgfx::TextArea lbl_instructions;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<screen_lockedViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // SCREEN_LOCKEDVIEWBASE_HPP