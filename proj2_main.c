/**
 * Starter code for Project 2. Good luck!
 *
 * We recommending copy/pasting your HAL folder from Project 1
 * into this project.
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <HAL/HAL.h>
#include <application.h>

#define FRAMERATE 10

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main()
{
    WDT_A_holdTimer();
    InitSystemTiming();

    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();

    Application app = Application_construct();
    HAL hal = HAL_construct();

    SWTimer framerate = SWTimer_construct(FRAMERATE);
    SWTimer_start(&framerate);

    while (1)
    {
        // Do not remove this line. This is your non-blocking check.
        PollNonBlockingLED();

        if(SWTimer_expired(&framerate))
        {
            HAL_refresh(&hal);
            Application_loop(&app, &hal);
            SWTimer_start(&framerate);
        }

    }
}
