from __future__ import annotations
from turtle import *
from math import sqrt, asin, degrees
from abc import ABC, abstractmethod


LINE_WIDTH = 2
DRAW_SPEED = 0
INVERTED_SIN_60 = 2 / sqrt(3)
HALF_OF_ANGLE = 30
EQUILATERAL_ANGLE = 120
STRAIGHT_ANGLE = 180

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
        color(args[0].Meta.color)
        begin_fill()
        output = func(*args, **kwargs)
        end_fill()
        return output
    return wrapper


class Vector2:
    def __init__(self, x: int = 0, y: int = 0):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    @property
    def val(self) -> tuple[int, int]:
        return self.x, self.y

    @staticmethod
    def xdiff(a, b) -> int:
        return b.x - a.x

    @staticmethod
    def ydiff(a, b) -> int:
        return b.y - a.y

    @staticmethod
    def diff(a, b) -> tuple[int, int]:
        return Vector2.xdiff(a, b), Vector2.ydiff(a, b)


class AbstractShape(ABC):
    @abstractmethod
    def draw(self):
        return NotImplemented


class Line(AbstractShape):
    class Meta:
        name = "отрезок"
        color = "#27083d"
        args = ["aX", "aY", "bX", "bY"]

    def __init__(self, x1: int, y1: int, x2: int, y2: int):
        a = Vector2(x1, y1)
        b = Vector2(x2, y2)

        if a == b:
            raise ValueError("Ошибка. Точки начала и конца совпадают.")

        self.a = a
        self.b = b

    @drawable
    def draw(self) -> Line:
        goto(self.b.val)
        return self


class Rectangle(Line):
    class Meta:
        name = "прямоугольник"
        color = "#00add7"
        args = ["aX", "aY", "bX", "bY"]

    @drawable
    def draw(self) -> Rectangle:
        goto(self.b.x, self.a.y)
        goto(self.b.x, self.b.y)
        goto(self.a.x, self.b.y)
        goto(self.a.x, self.a.y)
        return self


class Circle(AbstractShape):
    class Meta:
        name = "окружность"
        color = "#c1300d"
        args = ["X", "Y", "R"]

    def __init__(self, x: int, y: int, r: int):
        a = Vector2(x, y)
        self.a = a
        if r <= 0:
            print("Ошибка. Используйте положительный радиус")
        self.r = abs(r)

    @drawable
    def draw(self) -> Circle:
        circle(self.r)
        return self


class Triangle(Line):
    class Meta:
        name = "р\\с треугольник"
        color = "#e6d500"
        args = ["aX", "aY", "hX", "hY"]

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
        return int(a)

    @drawable
    def draw(self) -> Triangle:
        left(self.alpha + HALF_OF_ANGLE)

        for _ in range(3):
            forward(self.side)
            right(EQUILATERAL_ANGLE)

        right(self.alpha + HALF_OF_ANGLE)
        return self


shapes = [
    Line,
    Rectangle,
    Circle,
    Triangle
]
objects = []
FINISH_ACTION = len(shapes) + 1


def info_msg() -> str:
    output = "Выберите один из вариантов:\n"
    for index, i in enumerate(shapes):
        output += f"  {index + 1} - {i.Meta.name}\n"
    output += "  5 - Нарисовать\n"
    return output


while True:
    sel = input(info_msg())
    if not sel.isdigit():
        print("Пожалуйста, введите корректное числовое значение.")
        continue

    sel = int(sel)

    if sel == FINISH_ACTION:
        break

    if not 0 < sel < FINISH_ACTION:
        print("Нет такой опции.")
        continue

    shape = shapes[sel - 1]
    init_args = []
    for arg in shape.Meta.args:
        inp = input(f"Введите {arg}: ")
        try:
            inp = int(inp)

        except ValueError:
            print("Пожалуйста, введите корректное числовое значение.")
            break

        init_args.append(int(inp))

    else:
        obj = shape(*init_args)
        objects.append(obj)

for obj in objects:
    obj.draw()

done()
