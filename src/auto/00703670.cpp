#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, Polygon_GrowEdgeArray, 0x006D9D80,
         (void* self, void*, int extraSlots));
CSF_FUNC(char, __fastcall, S_00703360_GrowElementArray, 0x00703360,
         (void* self, void* edx, int a2));

static int __fastcall S_00703360_ResetEmitterSubObject(
    S_00703360* self, void* /*edx*/)
{
    // The packet labels the direct callee address as a vtable reference.
    (void)CSF_VTBL(0x00703360);

    auto* edgeArray = reinterpret_cast<uint8_t*>(self) + 0x10;
    const uint32_t edgeCapacity = self->field_0014;
    if (edgeCapacity < 4)
        Polygon_GrowEdgeArray(edgeArray, nullptr,
                              static_cast<int>(4 - edgeCapacity));

    CSF_AT(uint32_t, self, 0x18) = 4;

    const uint32_t elementCapacity = self->field_0004;
    if (elementCapacity < 4)
        S_00703360_GrowElementArray(
            self, nullptr, static_cast<int>(4 - elementCapacity));

    auto* elements = reinterpret_cast<uint8_t*>(self->field_0000);
    self->field_0008 = 4;
    CSF_AT(uint32_t, elements, 0x08) = 0;

    auto* edge = reinterpret_cast<uint32_t*>(
        CSF_AT(uint32_t, self, 0x10));
    edge[0] = 0;
    edge[1] = 0;
    edge[2] = 0;
    edge[3] = 0;

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0000), 0x18) = 0;
    edge = reinterpret_cast<uint32_t*>(
        CSF_AT(uint32_t, self, 0x10) + 0x10);
    edge[0] = 0;
    edge[1] = 0;
    edge[2] = 0;
    edge[3] = 0;

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0000), 0x28) = 0;
    edge = reinterpret_cast<uint32_t*>(
        CSF_AT(uint32_t, self, 0x10) + 0x20);
    edge[0] = 0;
    edge[1] = 0;
    edge[2] = 0;
    edge[3] = 0;

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0000), 0x38) = 0;
    edge = reinterpret_cast<uint32_t*>(
        CSF_AT(uint32_t, self, 0x10) + 0x30);
    edge[0] = 0;
    edge[1] = 0;
    edge[2] = 0;
    edge[3] = 0;

    return 0;
}

HOOK(0x00703670, S_00703360_ResetEmitterSubObject, "auto");
