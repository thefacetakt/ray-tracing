#ifndef RT_BASIC_GEOM
#define RT_BASIC_GEOM

#include "Float.hpp"

using namespace Float;

namespace BasicGeom {
    const int DIMENSIONS = 3;

    struct Vector {
        myFloat x, y, z;

        Vector();

        Vector(myFloat x, myFloat y, myFloat z);

        bool operator==(const Vector &a) const;

        bool operator!=(const Vector &a) const;

        Vector operator-(const Vector &a) const;

        Vector operator+(const Vector &a) const;

        myFloat operator*(const Vector &a) const;

        Vector operator%(const Vector &a) const;

        Vector operator-() const;

        Vector operator*(const myFloat &k) const;

        Vector operator/(const myFloat &k) const;

        myFloat len() const;

        myFloat len2() const;

        Vector normed() const;

        myFloat operator[](size_t i) const;
    };

    const Vector NONE = Vector(1e18, 1e18, 1e18);

    Vector::Vector() {
    }

    Vector::Vector(myFloat x, myFloat y, myFloat z) : x(x), y(y), z(z) {
    }

    bool Vector::operator==(const Vector &a) const {
        return eq(x, a.x) && eq(y, a.y) && eq(z, a.z);
    }

    bool Vector::operator!=(const Vector &a) const {
        return !(operator==(a));
    }

    Vector Vector::operator-(const Vector &a) const {
        return Vector(x - a.x, y - a.y, z - a.z);
    }

    Vector Vector::operator+(const Vector &a) const {
        return Vector(x + a.x, y + a.y, z + a.z);
    }

    myFloat Vector::operator*(const Vector &a) const {
        return x * a.x + y * a.y + z * a.z;
    }

    Vector Vector::operator%(const Vector &a) const {
        return Vector(y * a.z - z * a.y,
                      z * a.x - x * a.z,
                      x * a.y - y * a.x);
    }

    Vector Vector::operator-() const {
        return Vector(-x, -y, -z);
    }

    Vector Vector::operator*(const myFloat &k) const {
        return Vector(x * k, y * k, z * k);
    }

    Vector Vector::operator/(const myFloat &k) const {
        if (eq(k, 0.)) {
            return NONE;
        }
        return operator*(1. / k);
    }

    myFloat Vector::len() const {
        return sqrt(operator*(*this));
    }

    myFloat Vector::len2() const {
        return operator*(*this);
    }

    Vector Vector::normed() const {
        return operator/(len());
    }

    myFloat Vector::operator[](size_t i) const {
        if (i == 0) {
            return x;
        }
        if (i == 1) {
            return y;
        }
        return z;
    }

    bool collinearIfParralel(const Vector &a, const Vector &b) {
        return greaterOrEqual(a * b, 0.);
    }



    enum ERayConstructor {
        START_DIRECTION,
        START_POINT,
    };

    struct Ray {
        Vector start;
        Vector direction;
        Ray(const Vector &v1, const Vector &v2,
            ERayConstructor type=START_DIRECTION) {
            switch(type) {
                case START_DIRECTION:
                    start = v1;
                    direction = v2;
                break; case START_POINT:
                    start = v1;
                    direction = v2 - v1;
                break;
            }
            direction = direction / direction.len();
        }
    };

    struct Plane {
        Vector n;
        Vector start;

        Plane(const Vector &start, const Vector &a, const Vector &b)
            : start(start) {
            n = a % b;
            n = n / n.len();
        }

        Plane(const Vector &start, const Vector &n) : start(start) {
            this->n = n.normed();
        }

        myFloat d() const {
            return -start * n;
        }
    };

    myFloat det(const Vector &a, const Vector &b, const Vector &c) {
        return a.x * (b.y * c.z - b.z * c.y)
               + a.y * (b.z * c.x - b.x * c.z)
               + a.z * (b.x * c.y - b.y * c.x);
    }

    Vector ThreePlanesIntersection(const Plane &a, const Plane &b,
                                   const Plane &c) {
        myFloat d = det(a.n, b.n, c.n);
        if (eq(d, 0.)) {
            return NONE;
        }
        Vector dVector = Vector(-a.d(), -b.d(), -c.d());
        Vector x = Vector(a.n.x, b.n.x, c.n.x);
        Vector y = Vector(a.n.y, b.n.y, c.n.y);
        Vector z = Vector(a.n.z, b.n.z, c.n.z);
        myFloat d1 = det(dVector, y, z);
        myFloat d2 = det(x, dVector, z);
        myFloat d3 = det(x, y, dVector);
        return Vector(d1 / d, d2 / d, d3 / d);
    }

    Vector intersect(const Ray &ray, const Plane &plane) {
        Vector dir = ray.direction.normed();
        myFloat denominator = dir * plane.n;
        if (eq(denominator, 0.)) {
            return NONE;
        }
        myFloat t = -(ray.start * plane.n + plane.d()) / denominator;
        if (greaterOrEqual(t, 0.)) {
            return ray.start + dir * t;;
        }
        return NONE;
    }
};

#endif
