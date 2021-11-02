/* Author: Wyatt Geckle
 * Date: 9/30/21
 *
 * For use with any V5 drivetrain with optical sensor in front facing the floor.
 * Requires specific track pattern to function correctly: not all-purpose.
 * 
 * Defines fixed movement operations on the AGV.
 */

typedef enum
{
    DRIVETRAINLEFT = -1,
    DRIVETRAINRIGHT = 1
} drivetrain_dir;

void drivetrain_auto_drive(float vel_in_per_sec);

void drivetrain_auto_turn(float vel_in_per_sec, drivetrain_dir dir);

void drivetrain_stop();
