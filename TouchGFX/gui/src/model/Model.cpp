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

