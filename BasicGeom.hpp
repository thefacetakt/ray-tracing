#ifndef RT_BASIC_GEOM
#define RT_BASIC_GEOM

#include "Float.hpp"

using namespace Float;

namespace BasicGeom {

    struct Vector {
        float x, y, z;

        Vector();

        Vector(float x, float y, float z);

        bool operator==(const Vector &a) const;

        Vector operator-(const Vector &a) const;

        Vector operator+(const Vector &a) const;

        float operator*(const Vector &a) const;

        Vector operator%(const Vector &a) const;

        Vector operator-() const;

        Vector operator*(const float &k) const;

        Vector operator/(const float &k) const;

        float len() const;

        Vector normed() const;
    };

    const Vector NONE = Vector(1e18, 1e18, 1e18);

    Vector::Vector() {
    }

    Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {
    }

    bool Vector::operator==(const Vector &a) const {
        return eq(x, a.x) && eq(y, a.y) && eq(z, a.z);
    }

    Vector Vector::operator-(const Vector &a) const {
        return Vector(x - a.x, y - a.y, z - a.z);
    }

    Vector Vector::operator+(const Vector &a) const {
        return Vector(x - a.x, y - a.y, z - a.z);
    }

    float Vector::operator*(const Vector &a) const {
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

    Vector Vector::operator*(const float &k) const {
        return Vector(x * k, y * k, z * k);
    }

    Vector Vector::operator/(const float &k) const {
        if (eq(k, 0.)) {
            return NONE;
        }
        return operator*(1. / k);
    }

    float Vector::len() const {
        return sqrt(operator*(*this));
    }

    Vector Vector::normed() const {
        return operator/(len());
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
        }
    };

    float compareWithZeroAndReturn(float x, float result) {
        if (eq(x, 0.) {
            return 0.;
        }
        return result;
    }

    struct Plane {
        Vector n;
        Vector start;

        Plane(Vector start, Vector a, Vector b) : start(start) {
            n = a % b;
            n = n / n.len();
        }

        Vector getPoint() const {
            int notNullCoeffs = !eq(n.a, 0.) + !eq(n.b, 0.) + !eq(n.c, 0.);
            float d_ = d();
            return Vector(compareWithZeroAndReturn(n.a,
                                                   -d_ / (notNullCoeffs * n.a)),
                          compareWithZeroAndReturn(n.b,
                                                   -d_ / (notNullCoeffs * n.b)),
                          compareWithZeroAndReturn(n.c,
                                                   -d_ / (notNullCoeffs * n.c))
                          );
        }

        float d() const {
            return start * n;
        }
    };

    float det(const Vector &a, const Vector &b, const Vector &c) {
        return a.x * (b.y * c.z - b.z * c.y)
               + a.y * (b.z * c.x - b.x * c.z)
               + a.z * (b.x * c.y - b.y * c.x);
    }

    Vector ThreePlanesIntersection(const Plane &a, const Plane &b,
                                   const Plane &c) {
        float d = det(a.n, b.n, c.n);
        if (eq(d, 0.)) {
            return NONE;
        }
        Vector dVector = Vector(-a.d(), -b.d(), -c.d());
        Vector x = Vector(a.n.x, b.n.x, c.n.x);
        Vector y = Vector(a.n.y, b.n.y, c.n.y);
        Vector z = Vector(a.n.z, b.n.z, c.n.z);
        float d1 = det(dVector, y, z);
        float d2 = det(x, dVector, z);
        float d3 = det(x, y, dVector);
        return Vector(d1 / d, d2 / d, d3 / d);
    }
};

#endif
