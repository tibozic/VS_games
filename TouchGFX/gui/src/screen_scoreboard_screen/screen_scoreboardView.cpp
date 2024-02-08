#include <gui/screen_scoreboard_screen/screen_scoreboardView.hpp>

screen_scoreboardView::screen_scoreboardView()
{

}

void screen_scoreboardView::setupScreen()
{
    screen_scoreboardViewBase::setupScreen();
}

void screen_scoreboardView::tearDownScreen()
{
    screen_scoreboardViewBase::tearDownScreen();
}

void screen_scoreboardView::update_scoreboard(score_entry *scores)
{
	Unicode::UnicodeChar buffer[20];


	Unicode::strncpy(buffer, scores[0].player_name, 20);
	touchgfx::Unicode::snprintf(lbl_player_nameBuffer1, LBL_PLAYER_NAMEBUFFER1_SIZE, "%s", buffer);
	touchgfx::Unicode::snprintf(lbl_player_nameBuffer2, LBL_PLAYER_NAMEBUFFER2_SIZE, "%05u", scores[0].player_score);

	Unicode::strncpy(buffer, scores[1].player_name, 20);
	touchgfx::Unicode::snprintf(lbl_player_name_2Buffer1, LBL_PLAYER_NAME_2BUFFER1_SIZE, "%s", buffer);
	touchgfx::Unicode::snprintf(lbl_player_name_2Buffer2, LBL_PLAYER_NAME_2BUFFER2_SIZE, "%05u", scores[1].player_score);

	Unicode::strncpy(buffer, scores[2].player_name, 20);
	touchgfx::Unicode::snprintf(lbl_player_name_3Buffer1, LBL_PLAYER_NAME_3BUFFER1_SIZE, "%s", buffer);
	touchgfx::Unicode::snprintf(lbl_player_name_3Buffer2, LBL_PLAYER_NAME_3BUFFER2_SIZE, "%05u", scores[2].player_score);

	Unicode::strncpy(buffer, scores[3].player_name, 20);
	touchgfx::Unicode::snprintf(lbl_player_name_4Buffer1, LBL_PLAYER_NAME_4BUFFER1_SIZE, "%s", buffer);
	touchgfx::Unicode::snprintf(lbl_player_name_4Buffer2, LBL_PLAYER_NAME_4BUFFER2_SIZE, "%05u", scores[3].player_score);

	Unicode::strncpy(buffer, scores[4].player_name, 20);
	touchgfx::Unicode::snprintf(lbl_player_name_5Buffer1, LBL_PLAYER_NAME_5BUFFER1_SIZE, "%s", buffer);
	touchgfx::Unicode::snprintf(lbl_player_name_5Buffer2, LBL_PLAYER_NAME_5BUFFER2_SIZE, "%05u", scores[4].player_score);

	lbl_player_name.invalidateContent();
	lbl_player_name_2.invalidateContent();
	lbl_player_name_3.invalidateContent();
	lbl_player_name_4.invalidateContent();
	lbl_player_name_5.invalidateContent();
}
