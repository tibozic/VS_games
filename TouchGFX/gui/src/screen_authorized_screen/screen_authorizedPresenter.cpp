#include <gui/screen_authorized_screen/screen_authorizedView.hpp>
#include <gui/screen_authorized_screen/screen_authorizedPresenter.hpp>

screen_authorizedPresenter::screen_authorizedPresenter(screen_authorizedView& v)
    : view(v)
{

}

void screen_authorizedPresenter::activate()
{
	char *player_name = model->get_player_name();

	view.set_player_data(player_name, model->get_player_id());
}

void screen_authorizedPresenter::deactivate()
{

}
