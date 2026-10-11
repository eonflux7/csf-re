#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Fills a four-float record: [0] = a, [1] = a / b, [2] = 0, [3] = a / b.
// Returns the raw bits of `a` in eax.
static int __fastcall InitRatioVec4(float* self, void* /*edx*/, float a, float b)
{
    float ratio = a / b;
    self[0] = a;
    self[2] = 0.0f;
    self[1] = ratio;
    self[3] = ratio;

    uint32_t bits;
    std::memcpy(&bits, &a, sizeof(bits));
    return static_cast<int>(bits);
}

HOOK(0x0040A5E0, InitRatioVec4, "auto");
