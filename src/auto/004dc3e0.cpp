#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Thiscall method: self is the monitor object (this), a2 is unused by the original.
static char __fastcall UpdateActionMonitor(uint8_t* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0);
    if (owner == nullptr)
        return 0;

    uint8_t* ownerSub = CSF_AT(uint8_t*, owner, 0x14);
    void* checker = CSF_AT(void*, ownerSub, 0x18);
    if (checker == nullptr)
        return 0;

    // Virtual slot [edx+8] = vtable[2]; returns a bool in al.
    uint8_t checkerResult = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, checker, 0)[2])(checker, nullptr);
    if (checkerResult != 0)
        return 0;

    if (CSF_AT(uint8_t, self, 4) != 0)
        return 0;

    // Original re-reads [esi] after the virtual call.
    owner = CSF_AT(uint8_t*, self, 0);
    ownerSub = CSF_AT(uint8_t*, owner, 0x14);
    if (CSF_AT(uint8_t, ownerSub, 0x184) != 0)
    {
        CSF_AT(uint8_t, self, 4) = 1;
        CSF_AT(int32_t, owner, 0x48) = 0x40000000;
        owner = CSF_AT(uint8_t*, self, 0);
        CSF_AT(int32_t, owner, 0x40) = 2;
        return 0;
    }

    return 1;
}

HOOK(0x004DC3E0, UpdateActionMonitor, "auto");
