#include <gui/screen_authorized_screen/screen_authorizedView.hpp>

screen_authorizedView::screen_authorizedView()
{

}

void screen_authorizedView::setupScreen()
{
    screen_authorizedViewBase::setupScreen();
}

void screen_authorizedView::tearDownScreen()
{
    screen_authorizedViewBase::tearDownScreen();
}

void screen_authorizedView::set_player_data(char *name, int player_id)
{
	Unicode::UnicodeChar buffer[20];

	Unicode::strncpy(buffer, name, 20);
	Unicode::snprintf(lbl_nameBuffer, LBL_NAME_SIZE, "%s", buffer);

    Unicode::snprintf(lbl_tagBuffer, LBL_TAG_SIZE, "%05ud", player_id);

    lbl_tag.getParent()->invalidateContent();
	lbl_name.getParent()->invalidateContent();
}
