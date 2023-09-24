#ifndef TYPES_H
#define TYPES_H

#include <assert.h>
#include <math.h>
#include <stdio.h>

typedef double ElemType;

#undef  ElemTypeFormat
#define ElemTypeFormat "%lf"

static const ElemType POISON = NAN;

static inline int Equal(const ElemType* const a, const ElemType* const b)
{
    assert(a);
    assert(b);

    bool isfiniteA = isfinite(*a);
    bool isfiniteB = isfinite(*b);

    if (!isfiniteA & !isfiniteB)
        return 1;
    
    static const ElemType EPS = 1e-7;

    return fabs(*a - *b) < EPS;
}

#endif // TYPES_H