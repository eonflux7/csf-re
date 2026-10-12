#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));

// Builds an axis-aligned bounds object from a shape (box/sphere-like) object, depending on its type id.
static S_00402350* __cdecl Bounds_BuildFromShape(S_00402350* result, float* shape)
{
    alignas(8) uint8_t boundsBytes[0x60];
    alignas(8) uint8_t tempBytes[0x3C];
    void* bounds = boundsBytes;

    InitS00402350Object(bounds, nullptr);

    typedef int(__fastcall * GetTypeFn)(void*, void*);
    typedef void*(__fastcall * GetBoundsFn)(void*, void*, void*);

    int type = ((GetTypeFn)CSF_AT(void**, shape, 0)[2])(shape, nullptr);

    if (type == 0)
    {
        float x = shape[2];
        float y = shape[3];
        float z = shape[4];
        float r = shape[5];
        S_006abb80_InitializeBounds((S_006abb80*)bounds, nullptr,
                                    x + r, x - r,
                                    y + r, y - r,
                                    z + r, z - r);
    }
    else if (type == 1)
    {
        float x = shape[2];
        float y = shape[3];
        float z = shape[4];
        float rx = shape[5];
        float ry = shape[6];
        float rz = shape[7];
        S_006abb80_InitializeBounds((S_006abb80*)bounds, nullptr,
                                    x + rx, x - rx,
                                    y + ry, y - ry,
                                    z + rz, z - rz);
    }
    else if (type == 2)
    {
        void* src = ((GetBoundsFn)CSF_AT(void**, shape, 0)[7])(shape, nullptr, tempBytes);
        std::memcpy(bounds, src, 0x3C);
    }

    std::memcpy(result, bounds, 0x3C);
    return result;
}

HOOK(0x004025A0, Bounds_BuildFromShape, "auto");
