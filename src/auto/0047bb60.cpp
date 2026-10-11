#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns nonzero when the character's weapon action FSM refuses new weapon actions.
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));

// Looks up a character task by offset relative to the task object.
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Runs the weapon action on the owner's action FSM unless it is blocked; otherwise returns the blocked result.
static int __fastcall C_00834fa8_RunActionIfNotBlocked(C_00834fa8* self, void* /*edx*/, int /*a2*/)
{
    // Owner object pointer stored at this+0x08.
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

    // Action FSM pointer stored at owner+0x14.
    void* actionFsm = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));

    int result = CharActionFsm_IsWeaponActionBlocked(actionFsm, nullptr);
    if (!static_cast<uint8_t>(result))
    {
        // Embedded sub-object of the FSM at +4; its vtable slot 0x14 (index 5) is invoked.
        uint8_t* fsmSubObject = reinterpret_cast<uint8_t*>(actionFsm) + 4;
        void** subObjectVtbl = CSF_AT(void**, fsmSubObject, 0);
        ((void(__fastcall*)(void*, void*))subObjectVtbl[20 / 4])(fsmSubObject, nullptr);

        uint32_t taskOffset = CSF_AT(uint32_t, self, 0x0C);
        return CharTask_LookupByOffset(self, nullptr, static_cast<int>(taskOffset + 0x24));
    }
    return result;
}

HOOK(0x0047BB60, C_00834fa8_RunActionIfNotBlocked, "auto");
