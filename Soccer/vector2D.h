#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
struct Vector2D {
    T x;
    T y;

    Vector2D(T valX = 0, T valY = 0) : x(valX), y(valY) {}

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator-() const {
        return Vector2D(-x, -y);
    }

    void print() const {
        cout << "(" << x << ", " << y << ")" << endl;
    }

    double getAngle() const {
        return atan2(static_cast<double>(y), static_cast<double>(x));
    }
};

#endif