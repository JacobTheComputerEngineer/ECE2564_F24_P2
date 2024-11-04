/*
 * application.c
 *
 *  Created on: Oct 26, 2024
 *      Author: theen
 */

#include <application.h>

Application Application_construct()
{
    Application app;

    app.fsm=Title;

    int i; for(i=0; i<NumStates; i++) app.firstRun[i]=1;

    app.menu=Game;

    int j; for(j=0; j<NumHighScores; j++) app.hs[j]=999;

    app.x=XSTARTINGPOS;
    app.y=YSTARTINGPOS;

    app.lives=3;
    app.score=0;

    app.updateUI=0;

    for(i=0;i<5;i++)
    {
        app.ob[i].width=30;
        app.ob[i].height=100;
        app.ob[i].hit=0;
        app.ob[i].spawnRate=SWTimer_construct(RESPAWNRATE);
        app.ob[i].exists=0;
        if(i%2==0) app.ob[i].ypos=TopBorder;
        else app.ob[i].ypos=BottomBorder-app.ob[i].height;
    }

    app.spawnTimer = SWTimer_construct(SPAWNRATE);
    app.obIndex=0;

    return app;
}

void Application_loop(Application* app, HAL* hal)
{
    switch(app->fsm)
    {
    case Title:

        if(app->firstRun[Title])
        {
            app->firstRun[Title]=0;
            GFX_print(&hal->gfx, "Hello, world!", 0,0);

            SWTimer_start(&hal->timer);
        }

        if(SWTimer_expired(&hal->timer))
        {
            app->fsm=Menu;
        }

        break;
    case  Menu:

        if(app->firstRun[Menu])
        {
            app->firstRun[Menu]=0;
            GFX_clear(&hal->gfx);
            GFX_print(&hal->gfx, "Menu", 0,0);
            GFX_print(&hal->gfx, "--------------", 1,0);
            GFX_print(&hal->gfx, "Play game", 3,2);
            GFX_print(&hal->gfx, "High Scores", 4,2);
            GFX_print(&hal->gfx, "How to play", 5,2);

            GFX_print(&hal->gfx,"*",MenuVerticalOffset+app->menu, 0);   // cursor
        }

        handleMenuCursor(app, hal);

        if(hal->boosterpackJS.isTapped)
        {
            int state;
            state=app->menu+GameMenuStateDiff;
            app->fsm = (GameState)state;
            app->firstRun[Menu]=1;
        }

        break;
    case PlayGame:

        if(app->firstRun[PlayGame])
        {
            app->firstRun[PlayGame]=0;
            GFX_clear(&hal->gfx);
            GFX_print(&hal->gfx, "Game", 0,0);
            drawBoundaries(&hal->gfx);
            drawUI(app, hal);
            SWTimer_start(&app->spawnTimer);
        }

        game(app,hal);

        break;
    case HighScores:

        if(app->firstRun[HighScores])
        {
            app->firstRun[HighScores]=0;
            GFX_clear(&hal->gfx);
            GFX_print(&hal->gfx, "HighScores", 0,0);
            PrintHighScores(&hal->gfx, app->hs);
        }

        if(hal->boosterpackJS.isTapped)
        {
            app->fsm = Menu;
            app->firstRun[HighScores]=1;
        }

        break;
    case HowToPlay:

        if(app->firstRun[HowToPlay])
        {
            app->firstRun[HowToPlay]=0;
            GFX_clear(&hal->gfx);
            GFX_print(&hal->gfx, "HowToPlay", 0,0);
        }

        if(hal->boosterpackJS.isTapped)
        {
            app->fsm = Menu;
            app->firstRun[HighScores]=1;
        }

        break;
    case GameOver:
        break;
    }
}

void handleMenuCursor(Application* app, HAL* hal)
{

    if(hal->joy.up)
    {
        if(hal->joy.upTapped==0)
        {
            GFX_setForeground(&hal->gfx, BG_COLOR);
            GFX_print(&hal->gfx,"*",MenuVerticalOffset+app->menu, 0);

            if(app->menu!=0) app->menu--;
            else app->menu+=2;

            GFX_setForeground(&hal->gfx, FG_COLOR);
            GFX_print(&hal->gfx,"*",MenuVerticalOffset+app->menu, 0);
        }

        hal->joy.upTapped=1;
    }
    else hal->joy.upTapped=0;

    if(hal->joy.down)
    {

        if(hal->joy.downTapped==0)
        {
            GFX_setForeground(&hal->gfx, BG_COLOR);
            GFX_print(&hal->gfx,"*",MenuVerticalOffset+app->menu, 0);

            if(app->menu!=2) app->menu++;
                        else app->menu-=2;

            GFX_setForeground(&hal->gfx, FG_COLOR);
            GFX_print(&hal->gfx,"*",MenuVerticalOffset+app->menu, 0);
        }

        hal->joy.downTapped=1;

    }
    else hal->joy.downTapped=0;

}

void PrintHighScores(GFX* gfx, int hs[])
{
    int i;
    for(i=0;i<NumHighScores;i++)
        {
            char* score[16]={};
            snprintf((char *)score, 16, "%d", hs[i]);
            GFX_print(gfx, (char*)score, i+MenuVerticalOffset, 4);
            char printChars[2];
            printChars[0]=i+ASCIIOFFSET;
            printChars[1]='.';
            GFX_print(gfx, printChars,i+MenuVerticalOffset,0);
        }

}

void game(Application* app, HAL* hal)
{
    erasePlayer(&hal->gfx, app->x, app->y);

    if(app->y+BirdHeight+1<DownBorder && app->jumpFrameCounter==0) app->y++;

    if(app->x-1>LeftBorder && hal->joy.left) app->x--;

    if(app->x+BirdWidth+1<RightBorder && hal->joy.right) app->x++;

    if(hal->boosterpackS1.isTapped) app->jumpFrameCounter=jumpFrameCounterSize;

    if(app->jumpFrameCounter != 0)
    {
        app->jumpFrameCounter--;
        if(app->y -1> TopBorder) app->y--;
    }

    drawPlayer(&hal->gfx, app->x, app->y);

    if(SWTimer_expired(&app->spawnTimer))
    {
        app->ob[app->obIndex].exists=1;
        SWTimer_start(&app->spawnTimer);
    }

    int obCounter;
    for(obCounter=0;obCounter<5;obCounter++)
        {
            if(app->ob[obCounter].exists)
                {

                }
        }


}

void drawUI(Application* app, HAL* hal)
{
    GFX_print(&hal->gfx, "Score: ", 1,8);
    GFX_print(&hal->gfx, "Lives: ", 1,0);

    char* score[16]={};
    snprintf((char *)score, 16, "%d", app->score);
    GFX_print(&hal->gfx, (char*)score, 1, 14);

    char* lives[16]={};
    snprintf((char *)lives, 16, "%d", app->lives);
    GFX_print(&hal->gfx, (char*)lives, 1, 6);
}
