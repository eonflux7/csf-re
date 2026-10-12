#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GfxObject_GetBounds, 0x006C0C20, (int a1, int a5, float a3));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall C_00860288_UpdateBounds(C_00860288* self, void* /*edx*/)
{
    alignas(8) uint8_t boundsBytes[0x18];
    alignas(8) uint8_t centerBytes[0x18];

    if (!self->field_0060)
    {
        auto* storage = static_cast<S_00402350*>(Mem_Alloc(60));
        uint32_t* initialized = nullptr;
        if (storage)
        {
            (void)CSF_VTBL(0x00402350);
            initialized = S00402350_InitTransform(storage, nullptr);
        }
        self->field_0060 =
            static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(initialized));
    }

    const uint32_t modeBits = 1;
    float mode;
    std::memcpy(&mode, &modeBits, sizeof(mode));

    GfxObject_GetBounds(
        static_cast<int>(reinterpret_cast<std::intptr_t>(boundsBytes)),
        static_cast<int>(self->field_000c),
        mode);

    float* center = SceneMissionSetup_GetBoundsCenter(
        boundsBytes, nullptr, reinterpret_cast<float*>(centerBytes));

    const uint32_t centerX = CSF_AT(uint32_t, center, 0x0);
    void* destination = reinterpret_cast<void*>(self->field_0060);
    CSF_AT(uint32_t, destination, 0x0) = centerX;
    CSF_AT(uint32_t, destination, 0x4) = CSF_AT(uint32_t, center, 0x4);
    CSF_AT(uint32_t, destination, 0x8) = CSF_AT(uint32_t, center, 0x8);

    const float width =
        CSF_AT(float, boundsBytes, 0x0) - CSF_AT(float, boundsBytes, 0xC);
    auto* extents = reinterpret_cast<uint8_t*>(self->field_0060) + 0xC;
    const float height =
        CSF_AT(float, boundsBytes, 0x4) - CSF_AT(float, boundsBytes, 0x10);
    const float depth =
        CSF_AT(float, boundsBytes, 0x8) - CSF_AT(float, boundsBytes, 0x14);

    CSF_AT(float, extents, 0x0) = width * g_HalfDurationScale;
    const float halfHeight = height * g_HalfDurationScale;
    CSF_AT(float, extents, 0x4) = halfHeight;
    CSF_AT(float, extents, 0x8) = depth * g_HalfDurationScale;

    int result;
    std::memcpy(&result, &halfHeight, sizeof(result));
    return result;
}

HOOK(0x006B3A90, C_00860288_UpdateBounds, "auto");
