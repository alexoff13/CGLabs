from __future__ import annotations

from PIL import ImageDraw, Image


class Point:
    def __init__(self, x: int | float, y: int | float):
        self.x = x
        self.y = y


def digital_differential_analyzer(start: Point, end: Point, brush: ImageDraw) -> None:
    """
    Функция для отрисовки прямой, имея координаты начала и конца прямой

    :param start: Координата точки начала прямой
    :param end: Координата точки конца прямой
    :param brush: Объект, отрисовывающий картинку
    """
    slope = (end.y - start.y) / (end.x - start.y)
    point = Point(x=start.x, y=start.y + 0.5)

    while point.x <= end.x:
        brush.point((point.x, point.y), fill='black')
        point.y += slope
        point.x += 1


def bresenham(start: Point, end: Point, brush: ImageDraw) -> None:
    """
    Функция для отрисовки прямой с помощью алгоритма Брезенхема
    :param start: Координата точки начала прямой
    :param end: Координата точки конца прямой
    :param brush: Объект, отрисовывающий картинку
    """
    dx = end.x - start.x
    dy = end.y - start.y
    e = 2 * dy - dx
    incr_e = 2 * dy
    incr_ne = incr_e - 2 * dx

    point = Point(start.x, start.y)
    brush.point((point.x, point.y), fill='red')

    for _ in range(int(dx)):
        if e > 0:
            point.y += 1
            e += incr_ne
        else:
            e += incr_e
        point.x += 1
        brush.point((point.x, point.y), fill='red')


def circle_bresenham(center: Point, radius: int | float, brush: ImageDraw):
    point = Point(0, radius)
    d = 3 - 2 * radius
    draw_circle(center, point, brush)
    while point.y >= point.x:
        point.x += 1
        if d > 0:
            point.y -= 1
            d = d + 4 * (point.x - point.y) + 10

        else:
            d = d + 4 * point.x + 6
        draw_circle(center, point, brush)


def draw_circle(center: Point, point: Point, brush: ImageDraw):
    brush.point((center.x + point.x, center.y + point.y), 0)
    brush.point((center.x - point.x, center.y + point.y), 0)
    brush.point((center.x + point.x, center.y - point.y), 0)
    brush.point((center.x - point.x, center.y - point.y), 0)
    brush.point((center.x + point.y, center.y + point.x), 0)
    brush.point((center.x - point.y, center.y + point.x), 0)
    brush.point((center.x + point.y, center.y - point.x), 0)
    brush.point((center.x - point.y, center.y - point.x), 0)


if __name__ == '__main__':
    """
    реализовать растровые алгоритмы
    Алгоритм с использованием Fixed Point (DDA)
    Line: Алгоритм Брезенхема (метод центральной точки)
    Circle: Алгоритм Брезенхема (метод центральной точки)
    """
    im = Image.new('RGB', (500, 300), (255, 255, 255))
    drawer: ImageDraw = ImageDraw.Draw(im)

    digital_differential_analyzer(Point(1, 1), Point(100, 100), drawer)
    bresenham(Point(100, 50), Point(200, 250), drawer)
    circle_bresenham(Point(100, 100), 30, drawer)
    im.save('data/lab_2.png')
