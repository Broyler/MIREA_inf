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
EQUILATERAL_ANGLE = 120
STRAIGHT_ANGLE = 180
FINISH_ACTION = len(Shapes) + 1

width(LINE_WIDTH)
speed(DRAW_SPEED)

screen = getscreen()
w = screen.window_width()
h = screen.window_height()


def drawable(func):
    def wrapper(*args, **kwargs):
        up()
        goto(args[0].a.val)
        down()
        begin_fill()
        output = func(*args, **kwargs)
        end_fill()
        return output
    return wrapper


class Vector2:
    def __init__(self, x: int = 0, y: int = 0):
        self.x = x
        self.y = y

    @property
    def val(self) -> tuple[int, int]:
        return self.x, self.y

    def __add__(self, shift: Vector2) -> Vector2:
        new_vector = copy(self)
        new_vector.x += shift.x
        new_vector.y += shift.y
        return new_vector

    @staticmethod
    def xdiff(a, b):
        return b.x - a.x

    @staticmethod
    def ydiff(a, b):
        return b.y - a.y

    @staticmethod
    def diff(a, b):
        return Vector2.xdiff(a, b), Vector2.ydiff(a, b)


class AbstractShape(ABC):
    @abstractmethod
    def draw(self):
        return NotImplemented


class Line(AbstractShape):
    def __init__(self, a: Vector2, b: Vector2):
        self.a = a
        self.b = b

    @drawable
    def draw(self) -> Line:
        goto(self.b.val)
        return self


class Rectangle(Line):
    @drawable
    def draw(self) -> Rectangle:
        goto(self.b.x, self.a.y)
        goto(self.b.x, self.b.y)
        goto(self.a.x, self.b.y)
        goto(self.a.x, self.a.y)
        return self


class Circle(AbstractShape):
    def __init__(self, a: Vector2, r: int):
        self.a = a
        self.r = r

    @drawable
    def draw(self) -> Circle:
        circle(self.r)
        return self


class Triangle(Line):
    @property
    def height(self) -> int:
        a, b = Vector2.diff(self.a, self.b)
        height = sqrt(a ** 2 + b ** 2)
        return round(height)

    @property
    def side(self) -> int:
        a = self.height * INVERTED_SIN_60
        return round(a)

    @property
    def normal(self) -> int:
        a = Vector2.ydiff(self.a, self.b)
        return abs(a)

    @property
    def alpha(self) -> int:
        a = asin(self.normal / self.height)
        a = degrees(a)

        if a is None:
            raise ValueError

        return int(a)

    @drawable
    def draw(self) -> Triangle:
        left(self.alpha + HALF_OF_ANGLE)

        for _ in range(3):
            forward(self.side)
            right(EQUILATERAL_ANGLE)

        right(self.alpha + HALF_OF_ANGLE)
        return self


def line(x1, y1, x2, y2):
    a = Vector2(x1, y1)
    b = Vector2(x2, y2)
    a = Line(a, b).draw()

def rect(x1,y1,x2,y2):
    a = Vector2(x1, y1)
    b = Vector2(x2, y2)
    a = Rectangle(a, b).draw()

def circ(x1,y1,R):
    a = Circle(Vector2(x1, y1 - R), R).draw()

def tri(x1,y1,x2,y2):
    a = Vector2(x1, y1)
    b = Vector2(x2, y2)
    a = Triangle(a, b).draw()


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