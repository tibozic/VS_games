#include <gui/screen_scoreboard_screen/screen_scoreboardView.hpp>
#include <gui/screen_scoreboard_screen/screen_scoreboardPresenter.hpp>

screen_scoreboardPresenter::screen_scoreboardPresenter(screen_scoreboardView& v)
    : view(v)
{

}

void screen_scoreboardPresenter::activate()
{
	view.update_scoreboard(model->scores);
}

void screen_scoreboardPresenter::deactivate()
{

}

void screen_scoreboardPresenter::update_scoreboard()
{
	view.update_scoreboard(model->scores);
}
