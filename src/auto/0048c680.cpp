#include <cstdint>
#include <cmath>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// The original runs on the x87 stack in 80-bit extended precision: the sum, the
// limit and their difference are only rounded once, at the `fst(d)p` to memory.
// clang-cl's long double is 64-bit (MSVC ABI), so the extended values are kept
// as exact double-double pairs (hi + lo): every x87 input here is a 24-bit
// float, so two-sums in double capture the exact result the x87 keeps.
struct X87 {
    double hi;
    double lo;
};

static X87 two_sum(double a, double b)
{
    const double s = a + b;
    const double bb = s - a;
    const double err = (a - (s - bb)) + (b - bb);
    return X87{s, err};
}

// Extended addition of two floats (exact; the x87 rounds to 64 significand
// bits, which cannot change the later 24-bit store).
static X87 xadd(float a, float b)
{
    return two_sum(static_cast<double>(a), static_cast<double>(b));
}

static X87 xsub(const X87& a, const X87& b)
{
    // exact (hi,lo) difference of two exact pairs
    const X87 d = two_sum(a.hi, -b.hi);
    const X87 s1 = two_sum(d.lo, a.lo);
    const X87 s2 = two_sum(s1.hi, -b.lo);
    const double tail = s1.lo + s2.lo;
    X87 res = two_sum(d.hi, s2.hi);
    res.lo += tail;
    return res;
}

static bool xgreater(const X87& a, const X87& b)
{
    if (std::isnan(a.hi) || std::isnan(b.hi))
        return false;  // unordered: the original's `test ah,0x41` branches
    return a.hi > b.hi || (a.hi == b.hi && a.lo > b.lo);
}

static float bits_as_float(uint32_t bits)
{
    float f;
    std::memcpy(&f, &bits, 4);
    return f;
}

// Single rounding of the extended value to float (fst / fstp dword).
static float xtof(const X87& x)
{
    const float r = static_cast<float>(x.hi);
    const double d = (x.hi - static_cast<double>(r)) + x.lo;
    if (d == 0.0)
        return r;
    uint32_t rb, ub, db;
    std::memcpy(&rb, &r, 4);
    ub = rb + ((rb >> 31) ? 0xFFFFFFFFu : 1u);  // next after r toward +infinity
    db = rb - ((rb >> 31) ? 0xFFFFFFFFu : 1u);  // next after r toward -infinity
    if (rb == 0) {                              // +-0: neighbours are the denormals
        ub = 0x00000001u;
        db = 0x80000001u;
    }
    const float target = d > 0.0 ? bits_as_float(ub) : bits_as_float(db);
    const double half = (static_cast<double>(target) - static_cast<double>(r)) * 0.5;
    const double ad = d > 0.0 ? d : -d;
    if (ad > half)
        return target;
    if (ad < half)
        return r;
    // exact tie: round to even significand
    uint32_t tbits;
    std::memcpy(&tbits, &target, 4);
    return (tbits & 1) == 0 ? target : r;
}

static void __fastcall TCharTask_AdvanceSubObjectAccumulator(
    void* self, void* /*edx*/, float delta)
{
    const X87 sum = xadd(delta, CSF_AT(float, self, 0x04));
    CSF_AT(float, self, 0x04) = xtof(sum);

    const X87 limit = xadd(CSF_AT(float, self, 0x10), CSF_AT(float, self, 0x0C));

    if (xgreater(sum, limit))
        CSF_AT(float, self, 0x04) = xtof(xsub(sum, limit));
}

HOOK(0x0048C680, TCharTask_AdvanceSubObjectAccumulator, "auto");
