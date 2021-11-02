"""
Author: Wyatt Geckle
Date: 11/2/21

Generates G&M code. Writing G&M code the standard way doesn't allow for some useful function
calls, such as sine and cosine. It also doesn't allow for iteration.  Since there wasn't
an ellipse command I could find, I took it upon myself to draw one without writing every
point manually.
"""

from gandm_code_generator import GandMFile

import math

file = GandMFile("initials.nc")

#setup
file.generate_line("G90 G20", "ABSOLUTE COORD & INCH MODE")
file.generate_line("M06 T1", "TOOL CHANGE - LOAD TOOL 1")
file.generate_line("M03 S3000", "TURN SPINDLE ON CW TO 3000 RPM")
file.generate_line("G00 Z0.1", "RAPID TO 0.1 ABOVE PART")

#draw A
file.generate_line("G00 X0.5625 Y1.0", "RAPID TO POINT A")
file.generate_line("G01 Z-0.0625 F9", "PLUNGE 1/16 AT 9 IN/MIN")
file.generate_line("G01 X0.75 Y1.5 F18", "STRAIGHT LINE INTERP TO C AT 18 IN/MIN")
file.generate_line("G01 X0.9375 Y1.0", "STRAIGHT LINE INTERP TO E")
file.generate_line("G01 Z0.1", "RETRACT CUTTING TOOL")
file.generate_line("G00 X0.6445 Y1.125", "RAPID TO POINT B")
file.generate_line("G01 Z-0.0625 F9", "PLUNGE 1/16 AT 9 IN/MIN")
file.generate_line("G01 X0.8555 Y1.125 F18", "STRAIGHT LINE INTERP TO D AT 18 IN/MIN")
file.generate_line("G01 Z0.1", "RETRACT CUTTING TOOL")

#draw W
file.generate_line("G00 X1.0 Y1.5", "RAPID TO POINT F")
file.generate_line("G01 Z-0.0625 F9", "PLUNGE 1/16 AT 9 IN/MIN")
file.generate_line("G01 X1.125 Y1.0 F18", "STRAIGHT LINE INTERP TO G AT 18 IN/MIN")
file.generate_line("G01 X1.25 Y1.5", "STRAIGHT LINE INTERP TO H")
file.generate_line("G01 X1.375 Y1.0", "STRAIGHT LINE INTERP TO I")
file.generate_line("G01 X1.5 Y1.5", "STRAIGHT LINE INTERP TO J")
file.generate_line("G01 Z0.1", "RETRACT CUTTING TOOL")

#draw G
ellipse_center_x = (1.75 + 1.8125) / 2
ellipse_center_y = 1.25
ellipse_minor = ellipse_center_x - 1.625
ellipse_major = 0.25

file.generate_line("G00 X{} Y{}".format(
                    round(ellipse_minor * math.cos(math.radians(70)) + ellipse_center_x, 4),
                    round(ellipse_major * math.sin(math.radians(70)) + ellipse_center_y, 4)),
                   "RAPID TO POINT K")
file.generate_line("G01 Z-0.0625 F9", "PLUNGE 1/16 AT 9 IN/MIN")

file.generate_line("G01 X{} Y{} F18".format(
                    round(ellipse_minor * math.cos(math.radians(70)) + ellipse_center_x, 4),
                    round(ellipse_major * math.sin(math.radians(70)) + ellipse_center_y, 4)),
                   "BEGIN ELLIPSE TO POINT L AT 18 IN/MIN")
degree_step = 10
for theta in range(70 + degree_step, 360, degree_step):
    file.generate_line("G01 X{} Y{}".format(
                    round(ellipse_minor * math.cos(math.radians(theta)) + ellipse_center_x, 4),
                    round(ellipse_major * math.sin(math.radians(theta)) + ellipse_center_y, 4)))
file.generate_line("G01 X{} Y{}".format(ellipse_center_x + ellipse_minor, ellipse_center_y),
                   "END ELLIPSE TO POINT L")

file.generate_line("G01 X{} Y{}".format(ellipse_center_x + 0.03125, ellipse_center_y),
                   "STRAIGHT LINE INTERP TO M")
file.generate_line("G01 Z0.1", "RETRACT CUTTING TOOL")

#shutdown
file.generate_line("G00 Z1", "RAPID Z TO SAFE POSITION")
file.generate_line("G00 X0 Y3", "MOVE TABLE TO UNLOAD POSITION")
file.generate_line("M05", "TURN OFF SPINDLE")
file.generate_line("M02", "END OF PROGRAM")

file.close()
