from __future__ import annotations
from turtle import *
from math import sqrt, asin, degrees
from enum import Enum
from abc import ABC, abstractmethod
from copy import copy


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

width(LINE_WIDTH)
speed(DRAW_SPEED)

screen = getscreen()
w = screen.window_width()
h = screen.window_height()


class Vector2:
    def __init__(self, x: int = 0, y: int = 0):
        self.x = x
        self.y = y

    @property
    def val(self) -> tuple[int, int]:
        return self.x, self.y

    def __add__(self, shift: Self) -> Vector2:
        new_vector = copy(self)
        new_vector.x += shift.x
        new_vector.y += shift.y
        return new_vector


class AbstractShape(ABC):
    @abstractmethod
    def draw(self) -> None:
        return NotImplemented


class Line(AbstractShape):
    def __init__(self, A: Vector2, B: Vector2):
        self.A = A
        self.B = B

    def draw(self):
        up()
        goto(self.A.val)
        down()
        goto(self.B.val)
        return self


def coords(x, y):
    x = x - (w/2)
    y = y - (h/2)
    return x, y

def line(x1, y1, x2, y2):
    a = Vector2(x1, y1)
    b = Vector2(x2, y2)
    a = Line(a, b).draw()

def rect(x1,y1,x2,y2):
    begin_fill()
    line(x1,y1,x1,y2)
    line(x1,y2,x2,y2)
    line(x2,y2,x2,y1)
    line(x2,y1,x1,y1)
    end_fill()

def circ(x1,y1,R):
    x1, y1 = coords(x1, y1-R)
    penup()
    goto(x1,y1)
    begin_fill()
    pendown()
    circle(R)
    end_fill()

def tri(x1,y1,x2,y2):
    x1, y1 = coords(x1, y1)
    x2, y2 = coords(x2, y2)
    h = sqrt((x2-x1)**2 + (y2-y1)**2)
    a = h * INVERTED_SIN_60
    c = abs(y2 - y1)
    penup()
    goto(x1,y1)
    left(degrees(asin(c/h)) + HALF_OF_ANGLE)
    pendown()
    begin_fill()
    forward(a)
    right(EQUALITERAL_ANGLE)
    forward(a)
    right(EQUALITERAL_ANGLE)
    forward(a)
    left(STRAIGHT_ANGLE + HALF_OF_ANGLE - degrees((asin(c/h))))
    end_fill()


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
        color("#27083d")
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
        color("#00add7")
        rect(x1,y1,x2,y2)

    elif ch == str(Shapes.CIRCLE.value):
        try:
            x1 = int(input("X1 = "))
            y1 = int(input("Y1 = "))
            R = int(input("R = "))
        except:
            print("Ошибка ввода!!!")
            continue
        color("#c1300d")
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
        color("#e6d500")
        tri(x1,y1,x2,y2)

    elif ch == str(FINISH_ACTION):
        break

    else:
        print("Ошибка!!!")


done()