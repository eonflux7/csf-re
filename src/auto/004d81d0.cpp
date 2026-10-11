#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CharTask_IsAlternateStateActive, 0x004D7670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Returns the weapon-action-blocked result when set; otherwise looks up the
// child task at one of two state offsets depending on the alternate-state flag.
static int __fastcall C_00834fa8_LookupTaskByStateOffset(C_00834fa8* self, void* /*edx*/, int a2)
{
    // Owner object pointer stored at +0x08 of this task.
    uint32_t owner = CSF_AT(uint32_t, self, 0x08);

    // Action FSM pointer stored at +0x14 of the owner.
    void* actionFsm = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(owner), 0x14));

    int result = CharActionFsm_IsWeaponActionBlocked(actionFsm, nullptr);
    if (!static_cast<uint8_t>(result)) {
        if (CharTask_IsAlternateStateActive(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08)), nullptr)) {
            uint32_t stateBase = CSF_AT(uint32_t, self, 0x0C);
            return CharTask_LookupByOffset(self, nullptr, static_cast<int>(stateBase + 0x164));
        }
        uint32_t stateBase = CSF_AT(uint32_t, self, 0x0C);
        return CharTask_LookupByOffset(self, nullptr, static_cast<int>(stateBase + 0x54));
    }
    return result;
}

HOOK(0x004D81D0, C_00834fa8_LookupTaskByStateOffset, "auto");
