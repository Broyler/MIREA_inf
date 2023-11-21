import turtle
from math import sqrt, asin, degrees
from enum import Enum


class Shapes(Enum):
    LINE = 1
    RECTANGLE = 2
    CIRCLE = 3
    TRIANGLE = 4


LINE_WIDTH = 2
DRAW_SPEED = 0
INVERTED_SIN_60 = 2 / sqrt(3)
HALF_OF_ANGLE = 30
EQUALITERAL_ANGLE = 120
STRAIGHT_ANGLE = 180
FINISH_ACTION = len(Shapes) + 1


t = turtle.Turtle()
t.width(LINE_WIDTH)
sc = turtle.getscreen()
w, h = sc.window_width(), sc.window_height()
print(w, h)
t.speed(DRAW_SPEED)

def coords(x, y):
    x = x - (w/2)
    y = y - (h/2)
    return x, y

def line(x1, y1, x2, y2):
    x1, y1 = coords(x1, y1)
    x2, y2 = coords(x2, y2)
    t.penup()
    t.goto(x1, y1)
    t.pendown()
    t.goto(x2, y2)
    t.penup()

def rect(x1,y1,x2,y2):
    t.begin_fill()
    line(x1,y1,x1,y2)
    line(x1,y2,x2,y2)
    line(x2,y2,x2,y1)
    line(x2,y1,x1,y1)
    t.end_fill()

def circ(x1,y1,R):
    x1, y1 = coords(x1, y1-R)
    t.penup()
    t.goto(x1,y1)
    t.begin_fill()
    t.pendown()
    t.circle(R)
    t.end_fill()

def tri(x1,y1,x2,y2):
    x1, y1 = coords(x1, y1)
    x2, y2 = coords(x2, y2)
    h = sqrt((x2-x1)**2 + (y2-y1)**2)
    a = h * INVERTED_SIN_60
    c = abs(y2 - y1)
    t.penup()
    t.goto(x1,y1)
    t.left(degrees(asin(c/h)) + HALF_OF_ANGLE)
    t.pendown()
    t.begin_fill()
    t.forward(a)
    t.right(EQUALITERAL_ANGLE)
    t.forward(a)
    t.right(EQUALITERAL_ANGLE)
    t.forward(a)
    t.left(STRAIGHT_ANGLE + HALF_OF_ANGLE - degrees((asin(c/h))))
    t.end_fill()


while True:
    ch = input("Выберите фигуру:\n1 - Отрезок\n2 - Прямоугольник\n3 - Круг\n4 - Равносторонний треугольник\n5 - Нарисовать\n")
    if ch == str(Shapes.LINE.value):
        try:
            x1 = int(input("X1 = "))
            y1 = int(input("Y1 = "))
            x2 = int(input("X2 = "))
            y2 = int(input("Y2 = "))
        except:
            print("Ошибка ввода!!!")
            continue
        t.color("#27083d")
        line(x1,y1,x2,y2)

    elif ch == str(Shapes.RECTANGLE.value):
        try:
            x1 = int(input("X1 = "))
            y1 = int(input("Y1 = "))
            x2 = int(input("X2 = "))
            y2 = int(input("Y2 = "))
        except:
            print("Ошибка ввода!!!")
            continue
        t.color("#00add7")
        rect(x1,y1,x2,y2)

    elif ch == str(Shapes.CIRCLE.value):
        try:
            x1 = int(input("X1 = "))
            y1 = int(input("Y1 = "))
            R = int(input("R = "))
        except:
            print("Ошибка ввода!!!")
            continue
        t.color("#c1300d")
        circ(x1,y1,R)

    elif ch == str(Shapes.TRIANGLE.value):
        try:
            x1 = int(input("X1 = "))
            y1 = int(input("Y1 = "))
            x2 = int(input("X2 = "))
            y2 = int(input("Y2 = "))
        except:
            print("Ошибка ввода!!!")
            continue
        t.color("#e6d500")
        tri(x1,y1,x2,y2)

    elif ch == str(FINISH_ACTION):
        break

    else:
        print("Ошибка!!!")


turtle.done()