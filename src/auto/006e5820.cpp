#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Computes the larger/smaller ratio of two dimension values reached through
// this->field_000c -> +0x18 -> +0x20 -> ** -> *. Returns 0 if any link is null.
static int __fastcall C_00861ce0_ComputeAltRatio(C_00861ce0* self, void* /*edx*/)
{
    uint8_t* ownerObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c));
    uint32_t attachedObj = CSF_AT(uint32_t, ownerObj, 0x18);
    if (attachedObj == 0)
        return 0;

    uint8_t* attachedBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(attachedObj));
    uint32_t slotPtrHolder = CSF_AT(uint32_t, attachedBase, 0x20);

    uint32_t firstPtr = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(slotPtrHolder)), 0);
    if (firstPtr == 0)
        return 0;

    uint32_t secondPtr = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstPtr)), 0);
    if (secondPtr == 0)
        return 0;

    uint32_t statsObj = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(secondPtr)), 0);
    uint8_t* stats = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(statsObj));

    uint32_t valueA = CSF_AT(uint32_t, stats, 0x0C);
    uint32_t valueB = CSF_AT(uint32_t, stats, 0x10);

    if (valueA < valueB)
        return static_cast<int>(valueB / valueA);
    return static_cast<int>(valueA / valueB);
}

HOOK(0x006E5820, C_00861ce0_ComputeAltRatio, "auto");
