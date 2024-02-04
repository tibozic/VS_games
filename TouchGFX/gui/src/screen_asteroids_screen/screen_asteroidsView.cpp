#include <gui/screen_asteroids_screen/screen_asteroidsView.hpp>

screen_asteroidsView::screen_asteroidsView()
{

}

void screen_asteroidsView::setupScreen()
{
    screen_asteroidsViewBase::setupScreen();

    for(int i = 0; i < ROCK_COUNT; i++) {
    	rocks[i].setBitmap(touchgfx::Bitmap(BITMAP_ASTEROIDS_ROCK_ID));
    	rocks[i].setPosition(i*34, 79, 34, 29);
		rocks[i].setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
		rocks[i].setVisible(false);
		add(rocks[i]);
    }


    for(int i = 0; i < BULLET_COUNT; i++) {
    	bullets[i].setPosition(238, 219, 4, 4);
		bullets[i].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
		bullets[i].setVisible(false);
		add(bullets[i]);
    }

}

void screen_asteroidsView::tearDownScreen()
{
    screen_asteroidsViewBase::tearDownScreen();
}

void screen_asteroidsView::resume_asteroids_task()
{
	presenter->resume_asteroids_task();
}

void screen_asteroidsView::suspend_asteroids_task()
{
	presenter->suspend_asteroids_task();
}

void screen_asteroidsView::start_asteroids_game()
{
	presenter->start_asteroids_game();
}

void screen_asteroidsView::show_endgame_screen()
{
	presenter->hide_rocks();
	presenter->hide_bullets();
	container_game_over.setVisible(true);
	container_controls.setVisible(false);
	container_controls.getParent()->invalidateContent();
	container_game_over.getParent()->invalidateContent();
}

void screen_asteroidsView::hide_endgame_screen()
{
	container_game_over.setVisible(false);
	container_controls.setVisible(true);
	container_controls.getParent()->invalidateContent();
	container_game_over.getParent()->invalidateContent();
}

void screen_asteroidsView::update_hearts(char lives)
{
	if( lives == 0 ) {
		icon_heart3.setVisible(false);
		icon_heart2.setVisible(false);
		icon_heart1.setVisible(false);
	}
	else if( lives == 1 ) {
		icon_heart3.setVisible(false);
		icon_heart2.setVisible(false);
		icon_heart1.setVisible(true);
	}
	else if( lives == 2 ) {
		icon_heart3.setVisible(false);
		icon_heart2.setVisible(true);
		icon_heart1.setVisible(true);
	}
	else if( lives == 3 ) {
		icon_heart3.setVisible(true);
		icon_heart2.setVisible(true);
		icon_heart1.setVisible(true);
	}

	icon_heart3.invalidateContent();
	icon_heart2.invalidateContent();
	icon_heart1.invalidateContent();
}

void screen_asteroidsView::update_score(unsigned int score)
{
	Unicode::snprintf(lbl_scoreBuffer, LBL_SCORE_SIZE, "%05u", score);
	lbl_score.invalidateContent();
}

void screen_asteroidsView::new_rock_position(touchgfx::ScalableImage *rock)
{
	int random_x = rand() % (480 - rock->getWidth());
	int random_y = 0 - (rand() % 272);

	move_rock(rock, random_x, random_y);
	rock->setVisible(true);
	rock->getParent()->invalidateContent();
}

void screen_asteroidsView::getShipPosition(int *x, int *y)
{
	*x = icon_ship.getX();
	*y = icon_ship.getY();
}

Drawable* screen_asteroidsView::get_ship_pointer()
{
	return &icon_ship;
}

ShipDirection screen_asteroidsView::get_ship_direction()
{
	if( btn_up.getPressedState() ) {
		return ShipDirection::UP;
	}
	else if( btn_down.getPressedState() ) {
		return ShipDirection::DOWN;
	}
	else if( btn_left.getPressedState() ) {
		return ShipDirection::LEFT;
	}
	else if( btn_right.getPressedState() ) {
		return ShipDirection::RIGHT;
	}
	return NONE;
}

void screen_asteroidsView::set_ship_invincible(bool value)
{
	if( value ) {
		icon_shipPainter.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0));
	}
	else {
		icon_shipPainter.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
	}
}

void screen_asteroidsView::move_ship(int x, int y)
{
	icon_ship.setX(x);
	icon_ship.setY(y);
	icon_ship.getParent()->invalidateContent();
}

void screen_asteroidsView::move_rock(touchgfx::ScalableImage *rock, int x, int y)
{
	rock->setX(x);
	rock->setY(y);
	rock->getParent()->invalidateContent();
}

void screen_asteroidsView::move_bullet(touchgfx::Box *bullet, int x, int y)
{
	bullet->setX(x);
	bullet->setY(y);
	bullet->getParent()->invalidateContent();
}

void screen_asteroidsView::move_rock_offscreen(touchgfx::ScalableImage *rock)
{
	move_rock(rock, ROCK_OFFSCREEN_X, ROCK_OFFSCREEN_Y);
	rock->setVisible(false);
	rock->getParent()->invalidateContent();
}

void screen_asteroidsView::move_bullet_offscreen(touchgfx::Box *bullet)
{
	move_bullet(bullet, BULLET_OFFSCREEN_X, BULLET_OFFSCREEN_Y);
	bullet->setVisible(false);
	bullet->getParent()->invalidateContent();
}

bool screen_asteroidsView::bullet_button_pressed()
{
	return btn_shoot.getPressedState();
}

void screen_asteroidsView::hide_element(Drawable *element)
{
	element->setVisible(false);
	element->getParent()->invalidateContent();
}


