/* Author: Wyatt Geckle
 * Date: 9/30/21
 *
 * For use with any V5 drivetrain with optical sensor in front facing the floor.
 * Requires specific track pattern to function correctly: not all-purpose.
 * 
 * Defines fixed movement operations on the AGV.
 */

#include "main.h"
#include "sensor_port_defs.h"
#include "movement.h"

void drivetrain_auto_drive(float vel_in_per_sec)
{
    float wheel_radius = 1.35;  //inches
    
    //motor_rpm = vel in/sec * (60sec/1min) * (1rot/wheel_radius in)
    short int motor_rpm = (int)(vel_in_per_sec * 60 / wheel_radius);
    
    if (motor_rpm > 600)
        motor_rpm = 600;
    else if (motor_rpm < -600)
        motor_rpm = -600;
    
    motor_move_velocity(MOTORFRONTLEFT, motor_rpm);
    motor_move_velocity(MOTORFRONTRIGHT, motor_rpm);
    motor_move_velocity(MOTORBACKLEFT, motor_rpm);
    motor_move_velocity(MOTORBACKRIGHT, motor_rpm);
    
    return;
}



void drivetrain_auto_turn(float vel_in_per_sec, drivetrain_dir dir)
{
    float wheel_radius = 1.35;  //inches
    
    //motor_rpm = vel in/sec * (60sec/1min) * (1rot/wheel_radius in)
    short int motor_rpm = (int)(vel_in_per_sec * 60 / wheel_radius);
    
    if (motor_rpm > 600)
        motor_rpm = 600;
    else if (motor_rpm < -600)
        motor_rpm = -600;
        
    switch (dir)
    {
        case DRIVETRAINLEFT:
            motor_move_velocity(MOTORFRONTLEFT, -motor_rpm);
            motor_move_velocity(MOTORFRONTRIGHT, motor_rpm);
            motor_move_velocity(MOTORBACKLEFT, -motor_rpm);
            motor_move_velocity(MOTORBACKRIGHT, motor_rpm);
            break;
        
        case DRIVETRAINRIGHT:
            motor_move_velocity(MOTORFRONTLEFT, motor_rpm);
            motor_move_velocity(MOTORFRONTRIGHT, -motor_rpm);
            motor_move_velocity(MOTORBACKLEFT, motor_rpm);
            motor_move_velocity(MOTORBACKRIGHT, -motor_rpm);
    }
    
    return;
}



void drivetrain_stop()
{
    motor_move_voltage(MOTORFRONTLEFT, 0);
    motor_move_voltage(MOTORFRONTRIGHT, 0);
    motor_move_voltage(MOTORBACKLEFT, 0);
    motor_move_voltage(MOTORBACKRIGHT, 0);
    
    return;
}
