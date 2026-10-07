#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00863ba8_SetVectorAndMarkChanged(
    C_00863ba8* self, void* /*edx*/, const uint32_t* vector)
{
    const uint32_t first = vector[0];
    const uint32_t second = vector[1];
    const uint32_t third = vector[2];

    void* destination = reinterpret_cast<void*>(self->field_0008);
    CSF_AT(uint32_t, destination, 0x28) = first;
    CSF_AT(uint32_t, destination, 0x2C) = second;
    CSF_AT(uint32_t, destination, 0x30) = third;

    destination = reinterpret_cast<void*>(self->field_0008);
    const uint32_t flags = CSF_AT(uint32_t, destination, 0x70) | 1u;
    CSF_AT(uint32_t, destination, 0x70) = flags;
    return static_cast<int>(flags);
}

HOOK(0x00710E50, C_00863ba8_SetVectorAndMarkChanged, "auto");
