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
	model->resume_asteroids_task();
}

void screen_asteroidsPresenter::suspend_asteroids_task()
{
	model->suspend_asteroids_task();
}


bool screen_asteroidsPresenter::is_asteroids_game_started()
{
	return game_started;
}

void screen_asteroidsPresenter::start_asteroids_game()
{
	init_game();

}


void screen_asteroidsPresenter::init_game()
{
	/* SHIP STUFF */
	// move the ship to the starting position
	view.move_ship(225, 227);

	// move all the bullets off screen (bullets are on the right of the screen)
	// hide all the bullets

	for(int i = 0; i < BULLET_COUNT; i++ ) {
		view.move_bullet(view.bullets[i], BULLET_OFFSCREEN_X, BULLET_OFFSCREEN_Y);
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
		if( !rock->isVisible() )
			continue;

		view.move_rock(rock, rock->getX(), rock->getY() + ROCK_MOVES_PER_TICK);

		if( rock->getY() > SCREEN_HEIGHT ) {
			view.move_rock_offscreen(rock);
		}
		rock->getParent()->invalidateContent();
	}
}

void screen_asteroidsPresenter::check_ship_collisions()
{
	// get ship pointer
	Drawable *ship = view.get_ship_pointer();

	// check if ship collides with any of the rocks
	for(int i = 0; i < ROCK_COUNT; i++) {
		if( !view.rocks[i].isVisible() )
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
			}
		}
	}
}

void screen_asteroidsPresenter::game_over()
{

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








