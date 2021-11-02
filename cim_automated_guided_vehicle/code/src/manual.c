/* Author: Wyatt Geckle
 * Date: 9/30/21
 *
 * For use with any V5 drivetrain with optical sensor in front facing the floor.
 * Requires specific track pattern to function correctly: not all-purpose.
 * 
 * Defines manual mode operations on the AGV.
 */

#include "main.h"
#include "sensor_port_defs.h"
#include "auto.h"

void drivetrain_manual()
{
    signed char left_voltage;
    signed char right_voltage;
    unsigned char x_loops = 0;
    
    
    controller_clear(E_CONTROLLER_MASTER);
    delay(50);
    
    
    controller_print(E_CONTROLLER_MASTER,
                     0,
                     0,
                     "Auto: X for 1 sec");
    delay(50);
    
    
    //holding x for 1 second will prompt automatic mode
    while (x_loops < 200)
    {
        if (controller_get_digital(E_CONTROLLER_MASTER, DIGITAL_X))
            x_loops++;
        else
            x_loops = 0;
            
            
        //hit up to turn on optical sensor light, hit down to turn it off
        //
        //used for starting line alignment
        if (controller_get_digital(E_CONTROLLER_MASTER, DIGITAL_UP))
            optical_set_led_pwm(OPTICALSENSOR, 100);
        else if (controller_get_digital(E_CONTROLLER_MASTER, DIGITAL_DOWN))
            optical_set_led_pwm(OPTICALSENSOR, 0);
            
        
        left_voltage = controller_get_analog(E_CONTROLLER_MASTER, ANALOG_LEFT_Y);
        right_voltage = controller_get_analog(E_CONTROLLER_MASTER, ANALOG_RIGHT_Y);
        
        //base speed is half throttle, L2 halves base speed, R2 is full throttle
        if (controller_get_digital(E_CONTROLLER_MASTER, DIGITAL_L2))
        {
            left_voltage = left_voltage >> 2;
            right_voltage = right_voltage >> 2;
        }
        else if (!controller_get_digital(E_CONTROLLER_MASTER, DIGITAL_R2))
        {
            left_voltage = left_voltage >> 1;
            right_voltage = right_voltage >> 1;
        }
        
        motor_move(MOTORFRONTLEFT, left_voltage);
        motor_move(MOTORFRONTRIGHT, right_voltage);
        motor_move(MOTORBACKLEFT, left_voltage);
        motor_move(MOTORBACKRIGHT, right_voltage);
        
        delay(5);
    }
    
    
    //If A is pressed 3 times, enter auto mode. If B is pressed, go back to manual mode
    if (drivetrain_auto_startup())
        drivetrain_auto();
    else
        drivetrain_manual();
}
