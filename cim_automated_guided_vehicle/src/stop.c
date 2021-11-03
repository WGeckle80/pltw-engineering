/* Author: Wyatt Geckle
 * Date: 9/30/21
 *
 * For use with any V5 drivetrain with optical sensor in front facing the floor.
 * Requires specific track pattern to function correctly: not all-purpose.
 * 
 * Defines emergency stop on the AGV.
 */

#include "main.h"
#include "sensor_port_defs.h"

void emergency_stop()
{
    //emergency stop, stop all moving components
    
    motor_move_voltage(MOTORFRONTLEFT, 0);
    motor_move_voltage(MOTORFRONTRIGHT, 0);
    motor_move_voltage(MOTORBACKLEFT, 0);
    motor_move_voltage(MOTORBACKRIGHT, 0);
    
    printf("Stopped all moving components");
    
    
    bool button_state;
    bool button_hold = false;
    unsigned char num_a_presses = 0;
    
    
    //clears controller for ease
    controller_clear(E_CONTROLLER_MASTER);
    delay(50);
    
    
    controller_print(E_CONTROLLER_MASTER,
                     0,
                     0,
                     "Driving disabled");
    delay(50);
    controller_print(E_CONTROLLER_MASTER,
                     2,
                     0,
                     "Resume: Press A x3");
    delay(50);
    
    
    //press A 3 times to resume normal function
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
        
        delay(5);
    }
    
    
    controller_clear(E_CONTROLLER_MASTER);
    delay(50);
    
    
    //The following is printed on the controller:
    //Resuming in:
    //#seconds remaining in countdown
    //Stand Clear
    controller_print(E_CONTROLLER_MASTER,
                     0,
                     0,
                     "Resuming in:");
    delay(50);
    
    controller_print(E_CONTROLLER_MASTER,
                     2,
                     0,
                     "Stand clear");
    delay(50);
    
    //counts down for 5 seconds before resuming
    for (int sec = 5; sec >= 1; sec--)
    {
        controller_print(E_CONTROLLER_MASTER,
                         1,
                         0,
                         "%d",
                         sec);
        delay(1000);
    }
    
    controller_clear(E_CONTROLLER_MASTER);
    delay(50);
    
    return;
}
