#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, VecGrow_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));
CSF_GLOBAL(float, g_MinPointDistanceSq, 0x0082E6C4);

// Appends a 3-float point to the vector stored at +4 (data, capacity, size, allocator arg).
static void S_0041e750_AppendPoint(S_0041e750* self, const uint32_t* point)
{
    uint32_t* vec = &self->field_0004;
    if (vec[2] >= vec[1]) {
        VecGrow_4DE9B0(vec, nullptr, CSF_AT(int, self, 0x10));
    }
    uint32_t index = vec[2];
    uint32_t* data = reinterpret_cast<uint32_t*>(vec[0]);
    vec[2] = index + 1;
    uint32_t* dst = data + index * 3;
    dst[0] = point[0];
    dst[1] = point[1];
    dst[2] = point[2];
}

static void __fastcall S_0041e750_AddPointIfFarEnough(S_0041e750* self, void* /*edx*/, uint32_t* point)
{
    if (self->field_000c > 0) {
        const float* data = reinterpret_cast<const float*>(self->field_0004);
        const float* last = data + (self->field_000c * 3) - 3;
        const float* p = reinterpret_cast<const float*>(point);
        float dx = last[0] - p[0];
        float dy = last[1] - p[1];
        float dz = last[2] - p[2];
        float distSq = dx * dx + dy * dy + dz * dz;
        if (distSq > g_MinPointDistanceSq) {
            S_0041e750_AppendPoint(self, point);
        }
    } else {
        S_0041e750_AppendPoint(self, point);
    }
}

HOOK(0x0041ED90, S_0041e750_AddPointIfFarEnough, "auto");
