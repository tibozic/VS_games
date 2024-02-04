#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "cmsis_os.h"

extern "C" {
	extern osSemaphoreId_t resumeAsteroidsTaskSemaphoreHandle;
	extern osSemaphoreId_t suspendAsteroidsTaskSemaphoreHandle;
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
	modelListener->move_ship();
	//modelListener->move_bullets();
	modelListener->move_rocks();
	//modelListener->check_bullet_collisions();
	modelListener->check_ship_collisions();
}

void Model::resume_asteroids_task()
{
	osSemaphoreAcquire(resumeAsteroidsTaskSemaphoreHandle, 0U);
}


void Model::suspend_asteroids_task()
{
	osSemaphoreAcquire(suspendAsteroidsTaskSemaphoreHandle, 0U);
}

