#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(float*, __cdecl, RwBBoxAddPoint, 0x0063C9B0, (float* a1, float* a2));
CSF_FUNC(int, __fastcall, S_007010e0_FinalizeBounds, 0x007010E0, (void* self, void* edx));

static char __fastcall S_007010e0_InitializeBounds(
    S_007010e0* self, void* /*edx*/, int parameters)
{
    auto* source = reinterpret_cast<uint8_t*>(parameters);

    self->field_0004 = CSF_AT(uint32_t, source, 0x4);
    self->field_0008 = CSF_AT(uint32_t, source, 0x8);
    const uint32_t upperHeight = CSF_AT(uint32_t, source, 0xC);
    auto* geometry = reinterpret_cast<C_008310b8*>(self->field_0004);
    self->field_000c = upperHeight;
    self->field_002c = CSF_AT(uint8_t, source, 0x10);

    void* vertices = C_008310b8_GetSelf(geometry, nullptr);
    const uint32_t lowerHeight = CSF_AT(uint32_t, source, 0x8);
    auto* coordinates = CSF_AT(uint8_t*, vertices, 0x0);
    const uint32_t firstX = CSF_AT(uint32_t, coordinates, 0x0);
    const uint32_t firstZ = CSF_AT(uint32_t, coordinates, 0x4);

    alignas(8) uint8_t pointBytes[0xC];
    auto* point = reinterpret_cast<float*>(pointBytes);
    auto* bounds = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(self) + 0x10);

    CSF_AT(uint32_t, self, 0x10) = firstX;
    CSF_AT(uint32_t, pointBytes, 0x0) = firstX;
    CSF_AT(uint32_t, pointBytes, 0x4) = lowerHeight;
    CSF_AT(uint32_t, pointBytes, 0x8) = firstZ;
    CSF_AT(uint32_t, self, 0x14) = lowerHeight;
    CSF_AT(uint32_t, self, 0x18) = firstZ;
    CSF_AT(uint32_t, self, 0x1C) = firstX;
    CSF_AT(uint32_t, self, 0x20) = lowerHeight;
    CSF_AT(uint32_t, self, 0x24) = firstZ;

    for (uint32_t index = 1;
         index < CSF_AT(uint32_t, vertices, 0x8);
         ++index)
    {
        auto* coordinate = CSF_AT(uint8_t*, vertices, 0x0) + 8 * index;
        CSF_AT(float, pointBytes, 0x8) = CSF_AT(float, coordinate, 0x4);
        const uint32_t x = CSF_AT(uint32_t, coordinate, 0x0);
        CSF_AT(uint32_t, pointBytes, 0x4) = CSF_AT(uint32_t, source, 0xC);
        CSF_AT(uint32_t, pointBytes, 0x0) = x;
        RwBBoxAddPoint(bounds, point);
    }

    // Packet-listed vtable marker; the actual instruction calls this address.
    (void)CSF_VTBL(0x007010E0);
    S_007010e0_FinalizeBounds(self, nullptr);
    return 1;
}

HOOK(0x007011D0, S_007010e0_InitializeBounds, "auto");
