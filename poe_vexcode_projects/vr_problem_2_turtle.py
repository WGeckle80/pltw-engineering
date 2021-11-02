import turtle
import numpy as np
import math

#set default angle measure of turtle to radians to avoid too many conversions
turtle.radians()

def points_draw(list_of_coordinates, color):
    #make sure list_of_coordinates is a 2D list with x in the first row and y in the second

    length_of_list_x = list_of_coordinates[0].size

    #make sure the lists are of the same length
    if length_of_list_x != list_of_coordinates[1].size:
        return

    turtle.pendown()
    turtle.color(color)

    for i in range(length_of_list_x):
        x_dist = list_of_coordinates[0][i] - turtle.xcor()
        y_dist = list_of_coordinates[1][i] - turtle.ycor()

        dist_to_travel = math.sqrt(x_dist**2 + y_dist**2)
        theta = math.atan2(y_dist, x_dist)

        turtle.setheading(theta)
        turtle.forward(dist_to_travel)

    turtle.color("black")
    turtle.penup()

def main():
    turtle.penup()
    scaledown = 2 / 5

    turtle.setheading(math.pi)
    turtle.forward(200 * scaledown)

    #Lists can be pulled from anywhere, but for the purpose of this program it's just pre-defined
    #I'm not rewriting any of these coordinates so I just have them scaled down
    coordinates = np.array([
        [-400, -400, -350, -350, -400, -400, -150,
        175, 175, 125, 125, 325, 325, 275, 275, 325, 325, 125,
        -208, -258, -258, -207, -207],
        [0, 100, 100, 600, 600, 700, 700,
        137, 587, 587, 687, 687, 587, 587, 112, 112, 12, 12,
        588, 588, 103, 103, 0]
    ])
    coordinates = coordinates * scaledown
    points_draw(coordinates, "green")

    turtle.forward(197 * scaledown)

main()
turtle.done()  #keeps the window open until closed out