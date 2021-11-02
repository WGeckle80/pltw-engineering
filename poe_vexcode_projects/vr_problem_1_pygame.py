import pygame
import tkinter as tk
import math
import os

os.environ['SDL_VIDEO_CENTERED'] = '1'

pygame.init()

WIDTH = 500
HEIGHT = 500

screen = pygame.display.set_mode((WIDTH, HEIGHT))
clock = pygame.time.Clock()
FPS = 60
pygame.display.set_caption("VR Problem 1 Recreation")


class Dot:
    
    position = (25, HEIGHT - 25)
    
    x_to_travel = 0
    y_to_travel = 0
    
    x_speed = 0
    y_speed = 0
    
    current_digit_1 = 0
    current_digit_2 = 1
    
    numbers = []
    current_number = 0
    number_of_numbers = 0
    
    def set_number_of_numbers(self):
        self.number_of_numbers = len(self.numbers)
    
    def two_digits_of_number(self, number):
        #single digit number case
        if number < 10:
            return 0, number
        #these two are special cases if the last digit ends in 0
        elif number == 100:
            return 9, 10
        elif number % 10 == 0:
            return number / 10 - 1, 10
        #general case of two digit number
        else:
            this = str(number)  #convert to string so the digits can be indexed
            return int(this[0]), int(this[1])
        
    def next_number(self):
        next_number = self.numbers[self.current_number]
        next_digit_1, next_digit_2 = self.two_digits_of_number(next_number)

        x_tiles_to_travel = next_digit_2 - self.current_digit_2
        y_tiles_to_travel = next_digit_1 - self.current_digit_1

        self.x_to_travel = x_tiles_to_travel * 50
        self.y_to_travel = y_tiles_to_travel * 50

        theta = math.atan2(self.y_to_travel, self.x_to_travel)
        
        self.x_speed = 3 * math.cos(theta)
        self.y_speed = 3 * math.sin(theta)
        
        self.current_digit_1 = next_digit_1
        self.current_digit_2 = next_digit_2
        
    def update_position(self):
        x_difference = self.x_speed
        y_difference = self.y_speed
        
        if (abs(self.x_to_travel) < abs(x_difference)):
            x_difference = self.x_to_travel
        if abs(self.y_to_travel) < abs(y_difference):
            y_difference = self.y_to_travel
        
        self.position = (self.position[0] + x_difference,
                         self.position[1] - y_difference)
        
        self.x_to_travel -= x_difference
        self.y_to_travel -= y_difference
    
    def render(self):
        pygame.draw.circle(screen, (255, 0, 0), self.position, 5)


this_dot = Dot()



def submit_initial_number():
    try:
        this_dot.numbers.append(int(initial_number.get()))
        this_dot.numbers.append(int(math.sqrt(this_dot.numbers[-1])))
        this_dot.numbers.append(this_dot.numbers[-1] * 3)
        this_dot.numbers.append(this_dot.numbers[-1] // 2)
        this_dot.numbers.append(this_dot.numbers[-1] // this_dot.numbers[-1])
        this_dot.set_number_of_numbers()
        window.quit()
        window.destroy()
    except:
        print("Invalid")

window = tk.Tk()
window.title("Initial Number Selection")
number_label = tk.Label(window, text = "Integer(1-100):")
initial_number = tk.Entry(window)
submit = tk.Button(window, text = "Submit", command = submit_initial_number)

number_label.grid(row = 0, column = 0)
initial_number.grid(row = 0, column = 1)
submit.grid(row = 1, column = 0)

window.mainloop()


def draw():
    screen.fill((255, 255, 255))
    
    #draw the gridlines
    for x in range(50, WIDTH, 50):
        pygame.draw.line(screen, (0, 0, 0), (x, 0), (x, HEIGHT))
    for y in range(50, HEIGHT, 50):
        pygame.draw.line(screen, (0, 0, 0), (0, y), (WIDTH, y))
    
    #draw the numbers in each square
    font = pygame.font.SysFont(None, 24)
    for i in range(0, 10):
        for j in range(0, 10):
            current_number = font.render("{}".format(i * 10 + j + 1), True, (0, 0, 0))
            screen.blit(current_number, (50 * j + 20, HEIGHT - 50 * i - 25))
            
    
    this_dot.update_position()
    this_dot.render()
    #check if the dot has arrived to its destination, and make it go to its next location
    if (not this_dot.x_to_travel and not this_dot.y_to_travel) and (
            this_dot.current_number != this_dot.number_of_numbers):
        this_dot.next_number()
        this_dot.current_number += 1

gameRunning = True
while gameRunning:
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            gameRunning = False
            
    draw()
            
    pygame.display.update()
    clock.tick(FPS)
    
pygame.quit()