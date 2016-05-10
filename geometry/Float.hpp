#ifndef RT_FLOAT
#define RT_FLOAT

#include <cmath>

typedef long double myFloat;

namespace Float {
    const myFloat EPS = 1e-12;

    inline bool eq(myFloat a, myFloat b) {
        return fabsl(a - b) < EPS;
    }

    inline bool less(myFloat a, myFloat b) {
        return a + EPS < b;
    }

    inline bool lessOrEqual(myFloat a, myFloat b) {
        return a < b + EPS;
    }

    inline bool greater(myFloat a, myFloat b) {
        return less(b, a);
    }

    inline bool greaterOrEqual(myFloat a, myFloat b) {
        return lessOrEqual(b, a);
    }

    myFloat sq(myFloat a) {
        return a * a;
    }
};



#endif
