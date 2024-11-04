/*
 * application.h
 *
 *  Created on: Oct 26, 2024
 *      Author: theen
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <string.h>
#include <stdio.h>
#include <obstacle.h>

#define ASCIIOFFSET 48

#define NumStates 6
enum _GameState { Title, Menu, PlayGame, HighScores, HowToPlay, GameOver };
typedef enum _GameState GameState;

#define GameMenuStateDiff 2

#define MenuVerticalOffset 3
enum _MenuCursor { Game, LeaderBoard, Instructions};
typedef enum _MenuCursor MenuCursor;

#define NumHighScores 3

#define XSTARTINGPOS 63
#define YSTARTINGPOS 63

#define jumpFrameCounterSize 32

#define SPAWNRATE 2000

struct _Application
{
    GameState fsm;
    bool firstRun[NumStates];
    MenuCursor menu;

    int hs[NumHighScores];

    int x;
    int y;
    int lives;
    int score;
    int jumpFrameCounter;

    bool updateUI;

    Obstacle ob[5];
    SWTimer spawnTimer;
    int obIndex;
};
typedef struct _Application Application;

Application Application_construct();
void Application_loop(Application* app, HAL* hal);

void handleMenuCursor(Application* app, HAL* hal);

void PrintHighScores(GFX* gfx, int hs[]);

void game(Application* app, HAL* hal);
void drawUI(Application* app, HAL* hal);

#endif /* APPLICATION_H_ */
