/*
 * obstacle.h
 *
 *  Created on: Oct 30, 2024
 *      Author: theen
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include <HAL/hal.h>

#define RESPAWNRATE 5000

struct _Obstacle
{
    int width;
    int height;
    bool hit;
    //int speedCounter;
    SWTimer spawnRate;
    bool exists;
    int ypos;
    int xpos;
};
typedef struct _Obstacle Obstacle;

void drawOb(GFX* gfx, Obstacle ob);

#endif /* OBSTACLE_H_ */
