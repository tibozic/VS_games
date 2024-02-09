#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "cmsis_os.h"

extern "C" {
	extern osSemaphoreId_t bullet_timer_started_semaphoreHandle;
	extern osSemaphoreId_t bullet_timer_ended_semaphoreHandle;

	extern osSemaphoreId_t invincibility_timer_started_semaphoreHandle;
	extern osSemaphoreId_t invincibility_timer_ended_semaphoreHandle;
}

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
	if( modelListener->is_asteroids_game_started() ) {
		asteroids_tick();
	}
}

void Model::asteroids_tick() {
	bullet_timer_ended();
	invincibility_timer_ended();

	modelListener->move_ship();
	modelListener->move_bullets();
	modelListener->move_rocks();
	modelListener->check_bullet_collisions();
	modelListener->check_ship_collisions();
}

void Model::start_bullet_timer()
{
	osSemaphoreAcquire(bullet_timer_started_semaphoreHandle, 0U);
}

void Model::bullet_timer_ended()
{
	if( osSemaphoreGetCount(bullet_timer_ended_semaphoreHandle) == 0 ) {
		osSemaphoreRelease(bullet_timer_ended_semaphoreHandle);
		modelListener->set_allowed_to_shoot(true);
	}
}

void Model::start_invincibility_timer()
{
	osSemaphoreAcquire(invincibility_timer_started_semaphoreHandle, 0U);
}

void Model::invincibility_timer_ended()
{
	if( osSemaphoreGetCount(invincibility_timer_ended_semaphoreHandle) == 0 ) {
		osSemaphoreRelease(invincibility_timer_ended_semaphoreHandle);
		modelListener->set_invincible(false);
	}
}

void Model::login(int tag_id)
{
	char name[20];

	if( tag_id == 1 ) {
		strncpy(name, "Timi", 19);
	}
	else if( tag_id == 2 ) {
		strncpy(name, "Nejc", 19);
	}
	else {
		strncpy(name, "Guest", 19);
	}

	player_id = tag_id;
	strncpy(player_name, name, 20);
}

char* Model::get_player_name()
{
	return player_name;
}

int Model::get_player_id()
{
	return player_id;
}

void Model::update_scoreboard()
{
	modelListener->update_scoreboard();
}

void Model::resume_asteroids_task()
{
}


void Model::suspend_asteroids_task()
{
}

