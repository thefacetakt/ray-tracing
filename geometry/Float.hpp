#ifndef RT_FLOAT
#define RT_FLOAT

#include <cmath>

namespace Float {
    const float EPS = 1e-6;

    inline bool eq(float a, float b) {
        return fabsl(a - b) < EPS;
    }

    inline bool less(float a, float b) {
        return a + EPS < b;
    }

    inline bool lessOrEqual(float a, float b) {
        return a < b + EPS;
    }

    inline bool greater(float a, float b) {
        return less(b, a);
    }

    inline bool greaterOrEqual(float a, float b) {
        return lessOrEqual(b, a);
    }
};



#endif
