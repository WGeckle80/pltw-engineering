/* Author: Wyatt Geckle
 * Date: 9/30/21
 *
 * For use with any V5 drivetrain with optical sensor in front facing the floor.
 * Requires specific track pattern to function correctly: not all-purpose.
 * 
 * Defines automatic mode startup and operations on the AGV.
 */

#include <stdbool.h>

#include "main.h"
#include "sensor_port_defs.h"
#include "auto.h"
#include "movement.h"
#include "manual.h"

//auto mode starts when A button is pressed, cancelled if agv is not at starting line
//or if B button is pressed during window to press A 3 times
bool drivetrain_auto_startup()
{
    //if not at blue starting line, return false
    optical_set_led_pwm(OPTICALSENSOR, 100);
    delay(5);
    float hue = optical_get_hue(OPTICALSENSOR);
    optical_set_led_pwm(OPTICALSENSOR, 0);
    
    if (5.0 <= hue && hue < 120.0)
    {
        controller_print(E_CONTROLLER_MASTER,
                     2,
                     0,
                     "Not at starting line");
        delay(50);
        
        return false;
    }
    
    
    bool button_state;
    bool button_hold = false;
    unsigned char num_a_presses = 0;
    
    
    //clears controller for ease
    controller_clear(E_CONTROLLER_MASTER);
    delay(50);
    
    
    controller_print(E_CONTROLLER_MASTER,
                     0,
                     0,
                     "Start: Press A x3");
    delay(50);
    controller_print(E_CONTROLLER_MASTER,
                     1,
                     0,
                     "Cancel: Press B");
    delay(50);
    
    
    //wait until a is pressed 3 times or b is pressed
    while (num_a_presses < 3)
    {
        button_state = controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_A);
        
        if (!button_hold)
        {
            if (button_state)
            {
                button_hold = true;
                num_a_presses++;
            }
            else
                button_hold = false;
        }
        else
        {
            if (!button_state)
                button_hold = false;
        }
        
        if (controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_B))
        {
            return false;
        }
        
        delay(5);
    }
    
    
    controller_clear(E_CONTROLLER_MASTER);
    delay(50);
    
    
    controller_print(E_CONTROLLER_MASTER,
                     0,
                     0,
                     "Starting in:");
    delay(50);
    
    controller_print(E_CONTROLLER_MASTER,
                     2,
                     0,
                     "Stand clear");
    delay(50);
    
    //counts down for 5 seconds before starting automatic mode
    for (int sec = 5; sec >= 1; sec--)
    {
        controller_print(E_CONTROLLER_MASTER,
                         1,
                         0,
                         "%d",
                         sec);
        delay(1000);
    }
                     
    return true;
}


//automatic mode: holding X for one second returns to manual mode, drives straight on the
//center track color, turns right on the left track color, turns left on the right track color,
//and stops for 6 seconds on the intersection color.
//
//left is green, middle is yellow, right is red, and intersection is blue
void drivetrain_auto()
{
    unsigned short int x_loops = 0;
    
    float hue;
    unsigned char sat;
    
    float drive_speed = 1.6;  //in per sec
    
    float red_min = 5.0;
    float yellow_min = 40.0;
    float yellow_max = 55.0;
    float green_max = 120.0;
    float blue_min = 120.0;
    
    
    controller_clear(E_CONTROLLER_MASTER);
    delay(50);
    
    controller_print(E_CONTROLLER_MASTER,
                     0,
                     0,
                     "Manual: X for 1 sec");
    delay(50);
    
    
    //turns optical light on
    optical_set_led_pwm(OPTICALSENSOR, 100);
    
    
    //holding x for 1 second will return to manual mode
    while (x_loops < 1000)
    {
        if (controller_get_digital(E_CONTROLLER_MASTER, DIGITAL_X))
            x_loops++;
        else
            x_loops = 0;
        
        
        hue = optical_get_hue(OPTICALSENSOR);
        sat = (int)(optical_get_saturation(OPTICALSENSOR) * 10);
        
        //reference the color hue wheel
        if (red_min < hue && hue < yellow_min)
        {
            //red right edge
            drivetrain_auto_turn(drive_speed, DRIVETRAINLEFT);
        }
        else if (yellow_min <= hue && hue < yellow_max)
        {
            switch (sat)
            {
                case 8:
                case 9:
                    //yellow center
                    drivetrain_auto_drive(drive_speed);
                    break;
                default:
                    //most commonly veers off the left side
                    drivetrain_auto_turn(drive_speed, DRIVETRAINRIGHT);
            }
        }
        else if (yellow_max <= hue && hue < green_max)
        {
            //green left edge
            drivetrain_auto_turn(drive_speed, DRIVETRAINRIGHT);
        }
        else if (blue_min <= hue || hue < red_min)
        {
            //blue intersection
            drivetrain_stop();
            delay(6000);
            drivetrain_auto_drive(drive_speed);
            delay(500);
        }
        
        delay(1);
    }
    
    
    //turns off optical sensor light
    optical_set_led_pwm(OPTICALSENSOR, 0);
    
    
    drivetrain_manual();
}
