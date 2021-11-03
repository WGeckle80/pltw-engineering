/* Author: Wyatt Geckle
 * Date: 9/30/21
 *
 * For use with any V5 drivetrain with optical sensor in front facing the floor.
 * Requires specific track pattern to function correctly: not all-purpose.
 * 
 * Initializes v5 motors and emergency switch, makes sure controller is connected, and
 * handles emergency shutdown signals.
 */

#include <stdbool.h>

#include "main.h"
#include "sensor_port_defs.h"
#include "auto.h"
#include "manual.h"
#include "stop.h"


//The first thing to execute in the program
void initialize()
{
    //set emergency bumpswitch to input
    adi_pin_mode(EMERGENCYSWITCH, INPUT);
    
    //set motors to have maximum of 600 rpm
    motor_set_gearing(MOTORFRONTLEFT, E_MOTOR_GEARSET_06);
    motor_set_gearing(MOTORFRONTRIGHT, E_MOTOR_GEARSET_06);
    motor_set_gearing(MOTORBACKLEFT, E_MOTOR_GEARSET_06);
    motor_set_gearing(MOTORBACKRIGHT, E_MOTOR_GEARSET_06);
    
    //The left and right wheels need to have positive in opposite rotation directions
    motor_set_reversed(MOTORFRONTLEFT, false);
    motor_set_reversed(MOTORFRONTRIGHT, true);
    motor_set_reversed(MOTORBACKLEFT, false);
    motor_set_reversed(MOTORBACKRIGHT, true);
}


//executes after initialize finishes
void opcontrol()
{
    
    //requires controller connection to activate program.
    //
    //controller is only wireless way to shutdown the program, so it is required.
    if (!controller_is_connected(E_CONTROLLER_MASTER))
    {
        screen_print(TEXT_MEDIUM, 1, "Please connect a controller");
        
        while (!controller_is_connected(E_CONTROLLER_MASTER))
        {
            //stalls program until controller is connected
            delay(5);
        }
        
        screen_scroll(1, 1);
        
        delay(1000);
    }
    
    //main task while still allowing for emergency shutdowns if necessary
    task_t vehicle_task = task_create(drivetrain_manual,
                                      NULL,
                                      TASK_PRIORITY_DEFAULT,
                                      TASK_STACK_DEPTH_DEFAULT,
                                      "Vehicle Task");
    
    while (true)
    {
        //Checks if the controller disconnects, B, Down, L2, and R2 are held simultaneously on
        //the controller, or if the bumpswitch is hit. If any of the conditions are true,
        //stop all moving components.
        //
        //If the controller disconnects, there's no other wireless way to make an emergency
        //shutdown, which is why it will be required to be on as long as the program runs
        
        if (!controller_is_connected(E_CONTROLLER_MASTER))
        {
            task_suspend(vehicle_task);
            emergency_stop();
            task_resume(vehicle_task);
        }
        
        if (controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_B) &&
            controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_DOWN) &&
            controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_L2) &&
            controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_R2))
        {
            task_suspend(vehicle_task);
            emergency_stop();
            task_resume(vehicle_task);
        }
        
        if (adi_digital_read(EMERGENCYSWITCH))
        {
            task_suspend(vehicle_task);
            emergency_stop();
            task_resume(vehicle_task);
        }
        
        delay(5);
    }
}
