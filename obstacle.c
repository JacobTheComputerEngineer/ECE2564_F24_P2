/*
 * obstacle.c
 *
 *  Created on: Oct 30, 2024
 *      Author: theen
 */

#include <obstacle.h>

void drawOb(GFX* gfx, Obstacle ob)
{
    GFX_drawFilledRectangle(gfx, ob.xpos, ob.ypos, ob.width, ob.height);
}
