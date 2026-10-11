#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Returns the result of the virtual query on the owner's sub-object, using the owner's
// 32-bit value at +0x238 as the first argument and (0, -1, 0) as the remaining arguments.
static int __fastcall AiTaskStates_QueryOwnerSubobjectState(void* self, void* /*edx*/) {
    void* owner = CSF_AT(void*, self, 0x08);
    void* subObject = CSF_AT(void*, owner, 0x14);
    uint32_t ownerValue = CSF_AT(uint32_t, owner, 0x238);

    // Slot 0x8C / 4 = 35 on the sub-object's vtable.
    auto queryFn = (int(__fastcall*)(void*, void*, uint32_t, int, int, int))(CSF_AT(void**, subObject, 0)[35]);
    return queryFn(subObject, nullptr, ownerValue, 0, -1, 0);
}

HOOK(0x00420DC0, AiTaskStates_QueryOwnerSubobjectState, "auto");
