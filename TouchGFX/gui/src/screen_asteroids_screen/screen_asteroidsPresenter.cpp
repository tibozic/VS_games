#include <gui/screen_asteroids_screen/screen_asteroidsView.hpp>
#include <gui/screen_asteroids_screen/screen_asteroidsPresenter.hpp>

screen_asteroidsPresenter::screen_asteroidsPresenter(screen_asteroidsView& v)
    : view(v)
{

}

void screen_asteroidsPresenter::activate()
{

}

void screen_asteroidsPresenter::deactivate()
{

}


void screen_asteroidsPresenter::resume_asteroids_task()
{
}

void screen_asteroidsPresenter::suspend_asteroids_task()
{
}


bool screen_asteroidsPresenter::is_asteroids_game_started()
{
	return game_started;
}

void screen_asteroidsPresenter::start_asteroids_game()
{
	init_game();
}

int screen_asteroidsPresenter::new_score_position(unsigned int score)
{
	for(int i = 0; i < 5; i++) {
		if( score > model->scores[i].player_score)
			return i;
	}
	return -1;
}

void screen_asteroidsPresenter::game_over()
{
	char player_name[] = "Timi";
	game_started = false;

	// update the scoreboard
	int score_position = new_score_position(score);
	if( score_position != -1 ) {
		score_entry new_score_entry;
		strcpy(new_score_entry.player_name, player_name);
		new_score_entry.player_score = score;

		for(int i = 3; i > score_position; i--) {
			model->scores[i+1] = model->scores[i];
		}

		model->scores[score_position] = new_score_entry;
	}
	model->update_scoreboard();


	view.show_endgame_screen();
}

void screen_asteroidsPresenter::init_game()
{
	view.hide_endgame_screen();

	/* SHIP STUFF */
	// move the ship to the starting position
	view.move_ship(225, 227);

	// move all the bullets off screen (bullets are on the right of the screen)
	// hide all the bullets

	for(int i = 0; i < BULLET_COUNT; i++ ) {
		view.move_bullet(&view.bullets[i], BULLET_OFFSCREEN_X, BULLET_OFFSCREEN_Y);
		view.bullets[i].setVisible(false);
		view.bullets[i].getParent()->invalidateContent();
	}


	/* ROCK STUFF */
	// move all the rocks off screen (rock are to the left of the screen)
	// hide all the rocks
	for(int i = 0; i < ROCK_COUNT; i++ ) {
		view.move_rock(&view.rocks[i], ROCK_OFFSCREEN_X, ROCK_OFFSCREEN_Y);
		view.rocks[i].setVisible(false);
		view.rocks[i].getParent()->invalidateContent();
	}

	// generate random rock positions

	for(int i = 0; i < ROCK_COUNT; i++) {
		view.new_rock_position(&view.rocks[i]);
	}


	/* GENERAL GAME STUFF */
	// reset the score to 0
	score = 0;
	view.update_score(score);

	// reset health to full
	lives = 3;
	view.update_hearts(lives);

	// reset the ship
	set_invincible(false);

	// start the game
	game_started = true;
}

void screen_asteroidsPresenter::move_ship()
{
	int old_x, old_y;

	view.getShipPosition(&old_x, &old_y);

	int new_x = old_x;
	int new_y = old_y;

	ShipDirection ship_direction = view.get_ship_direction();

	switch(ship_direction) {
		case ShipDirection::LEFT: {
			new_x = old_x - SHIP_MOVES_PER_TICK;
			break;
		}
		case ShipDirection::RIGHT: {
			new_x = old_x + SHIP_MOVES_PER_TICK;
			break;
		}
		case ShipDirection::UP: {
			new_y = old_y - SHIP_MOVES_PER_TICK;
			break;
		}
		case ShipDirection::DOWN: {
			new_y = old_y + SHIP_MOVES_PER_TICK;
			break;
		}
		default: {
			break;
		}
	}

	Drawable *ship = view.get_ship_pointer();

	view.move_ship((new_x > (SCREEN_WIDTH - ship->getWidth()) || new_x < 0) ? old_x : new_x, (new_y > (SCREEN_HEIGHT - ship->getHeight()) || new_y < 0) ? old_y : new_y);
}


void screen_asteroidsPresenter::move_rocks()
{
	for(int i = 0; i < ROCK_COUNT; i++) {
		touchgfx::ScalableImage *rock = &view.rocks[i];
		if( !rock->isVisible() ) {
			// spawn new rock
			view.new_rock_position(rock);

			continue;
		}

		// move the rock
		view.move_rock(rock, rock->getX(), rock->getY() + ROCK_MOVES_PER_TICK);

		// check if rock is offscreen
		if( rock->getY() > SCREEN_HEIGHT ) {
			// remove the rock
			view.move_rock_offscreen(rock);
		}
		rock->getParent()->invalidateContent();
	}
}

Drawable *get_next_invisible_bullet()
{

	return NULL;
}

Drawable* screen_asteroidsPresenter::get_next_invisible_bullet()
{
	for(int i = 0; i < BULLET_COUNT; i++) {
		if( !view.bullets[i].isVisible() )
			return &view.bullets[i];
	}

	return NULL;
}

void screen_asteroidsPresenter::move_bullets()
{
	shoot_bullet();

	for(int i = 0; i < BULLET_COUNT; i++) {
		if( !view.bullets[i].isVisible() )
			continue;

		// move the bullet
		view.move_bullet(&view.bullets[i], view.bullets[i].getX(), view.bullets[i].getY() - BULLET_MOVES_PER_TICK);

		// check if bullet is offscreen
		if( view.bullets[i].getY() < 0 ) {
			// remove the offscreen bullet
			view.move_bullet_offscreen(&view.bullets[i]);
		}
	}
}

bool screen_asteroidsPresenter::is_allowed_to_shoot()
{
	return allowed_to_shoot;
}

void screen_asteroidsPresenter::set_allowed_to_shoot(bool value)
{
	allowed_to_shoot = value;
}

void screen_asteroidsPresenter::shoot_bullet()
{
	if( !is_allowed_to_shoot() )
		return;

	Drawable *ship = view.get_ship_pointer();

	// check if button for new bullet is pressed
	if( view.bullet_button_pressed() )
	{
		model->start_bullet_timer();
		set_allowed_to_shoot(false);

		// spawn new bullet
		Drawable *new_bullet = get_next_invisible_bullet();

		if( new_bullet != NULL )
		{
			new_bullet->setVisible(true);
			new_bullet->setX(ship->getX() + (ship->getWidth()/2) - 1);
			new_bullet->setY(ship->getY() + 1);
		}
	}
}

void screen_asteroidsPresenter::hide_rocks()
{
	for(int i = 0; i < ROCK_COUNT; i++) {
		view.hide_element(&view.rocks[i]);
	}
}


void screen_asteroidsPresenter::hide_bullets()
{
	for(int i = 0; i < BULLET_COUNT; i++) {
		view.hide_element(&view.bullets[i]);
	}
}

void screen_asteroidsPresenter::set_invincible(bool value)
{
	invincible = value;

	view.set_ship_invincible(invincible);
}

bool screen_asteroidsPresenter::is_invincible()
{
	return invincible;
}

void screen_asteroidsPresenter::check_ship_collisions()
{
	// get ship pointer
	Drawable *ship = view.get_ship_pointer();

	// check if ship collides with any of the rocks
	for(int i = 0; i < ROCK_COUNT; i++) {
		if( !view.rocks[i].isVisible() )
			continue;

		if( is_invincible() )
			continue;

		// if ship collides with rock the player loses a life
		if( elements_collide(ship, &view.rocks[i]) ) {
			// remove life
			lives--;
			view.update_hearts(lives);

			// remove the rock
			view.move_rock_offscreen(&view.rocks[i]);

			// if player has 0 lives game is over
			if( lives == 0 ) {
				game_over();
				return;
			}

			// trigger temporary invincibility
			model->start_invincibility_timer();
			set_invincible(true);
		}
	}
}

void screen_asteroidsPresenter::check_bullet_collisions()
{
	for(int i = 0; i < BULLET_COUNT; i++) {
		if( !view.bullets[i].isVisible() )
			continue;

		for(int j = 0; j < ROCK_COUNT; j++) {
			if( !view.rocks[j].isVisible() )
				continue;

			if(elements_collide(&view.bullets[i], &view.rocks[j])) {
				view.move_rock_offscreen(&view.rocks[j]);
				view.move_bullet_offscreen(&view.bullets[i]);

				score++;
				view.update_score(score);
			}
		}
	}
}

bool screen_asteroidsPresenter::elements_collide(Drawable *element1, Drawable *element2)
{
	if( element1->getX() > (element2->getX() + element2->getWidth()) || (element1->getX() + element1->getWidth()) < element2->getX()) {
		return false;
	}

	if( element1->getY() > (element2->getY() + element2->getHeight()) || (element1->getY() + element1->getHeight()) < element2->getY()) {
		return false;
	}

	return true;
}








