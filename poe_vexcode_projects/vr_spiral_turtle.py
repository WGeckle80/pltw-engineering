import turtle
import math
import numpy as np

#set default angle measure of turtle to radians to avoid conversions
turtle.radians()

def move_to(newx, newy):
    x_dist = newx - turtle.xcor()
    y_dist = newy - turtle.ycor()

    dist_to_travel = math.sqrt(x_dist**2 + y_dist**2)
    theta = math.atan2(y_dist, x_dist)

    turtle.setheading(theta)
    turtle.forward(dist_to_travel)

def points_draw(list_of_coordinates, color):
    #make sure list_of_coordinates is a 2D list with x in the first row and y in the second

    length_of_list_x = list_of_coordinates[0].size

    #make sure the lists are of the same length
    if length_of_list_x != list_of_coordinates[1].size:
        return

    #the first coordinate is the starting point of the drawing, so go there without drawing
    move_to(list_of_coordinates[0][0], list_of_coordinates[1][0])

    turtle.pendown()
    turtle.color(color)

    for i in range(1, length_of_list_x):
        #the x and y position of the drivetrain is just the element prior to the current one
        x_dist = list_of_coordinates[0][i] - list_of_coordinates[0][i - 1]
        y_dist = list_of_coordinates[1][i] - list_of_coordinates[1][i - 1]

        dist_to_travel = math.sqrt(x_dist**2 + y_dist**2)
        theta = math.atan2(y_dist, x_dist)

        turtle.setheading(theta)
        turtle.forward(dist_to_travel)

    turtle.color("black")
    turtle.penup()

def polar_to_cartesian(r, theta):
    #r is number, theta is array in radians
    
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    
    return np.array([x, y])

def draw_arc(r, theta, color):
    #r and theta are numbers, theta is in degrees
    
    #subtracts 90 so (0, 1) on the unit circle is tangent with pen moving up to it at 90 degrees
    current_angle = math.degrees(turtle.heading()) - 90
    theta += current_angle
    
    #list of angles so pen curves to the destination instead of moving shortest distance
    angle = np.array([x * (theta - current_angle) / 30 + current_angle for x in range(30 + 1)])
    
    #initial circular coordinates, based around (0, 0)
    coordinates = polar_to_cartesian(r, np.radians(angle))
    
    current_angle = math.radians(current_angle)
    #make sure the pen can begin drawing the arc immediately without moving into position
    coordinates[0] = coordinates[0] - r * math.cos(current_angle) + turtle.xcor()
    coordinates[1] = coordinates[1] - r * math.sin(current_angle) + turtle.ycor()
    
    points_draw(coordinates, color)
    
    #the pen will dealign without this
    turtle.setheading(math.radians(theta + 90))
    
def main():
    turtle.penup()
    scaledown = 2 / 5
    
    move_to(100, -200)
    turtle.setheading(math.pi / 2)
    
    #draw the first spiral, lines are red while arcs are blue
    forward_amount = 1000 * scaledown
    radius = 100 * scaledown
    for x in range(12):
        turtle.pendown()
        turtle.color("red")
        turtle.forward(forward_amount)
        draw_arc(radius, 150, "blue")
        
    move_to(110, -200)
    turtle.setheading(math.pi / 2)
    
    #draw the second spiral, lines and arcs are uniform, alternates between green and black
    forward_amount = 950 * scaledown
    radius = 90 * scaledown
    for x in range(6):
        turtle.pendown()
        turtle.color("green")
        turtle.forward(forward_amount)
        draw_arc(radius, 150, "green")
        
        turtle.pendown()
        turtle.color("black")
        turtle.forward(forward_amount)
        draw_arc(radius, 150, "black")
    
main()
turtle.done()  #keeps the window open until closed out