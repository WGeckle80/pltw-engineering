import turtle
import numpy as np
import math

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

def main():
    turtle.penup()
    scaledown = 2 / 5

    #I'm not rewriting any of these coordinates so I just have them scaled down
    coordinates = np.array([
        [0, 0, -200, -200, 200, 200, 0, 0],
        [100, 300, 300, 100, 100, -100, -100, 100]
    ])
    coordinates = coordinates * scaledown
    points_draw(coordinates, "green")

    coordinates = np.array([
        [141.4, 141.4, 70.7, -29.3, -100, -100, -29.3, 70.7, 141.4],
        [241.4, 341.4, 412.1, 412.1, 341.4, 241.4, 170.7, 170.7, 241.4]
    ])
    coordinates = coordinates * scaledown
    points_draw(coordinates, "red")

    step = 8
    theta = np.radians(np.array([x for x in range(0, 361, step)]))
    coordinates = np.array([
        379.75 * np.cos(theta) - 191.45,
        379.75 * np.sin(theta) + 338.3
    ])
    coordinates = coordinates * scaledown
    points_draw(coordinates, "blue")

    coordinates = np.array([
        [358.5, 358.5, -641.5, -641.5, 358.5],
        [-241.4, 758.6, 758.6, -241.4, -241.4]
    ])
    coordinates = coordinates * scaledown
    points_draw(coordinates, "black")
    
    turtle.forward(50)
    
main()
turtle.done()  #keeps the window open until closed out