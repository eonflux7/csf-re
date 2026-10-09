#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x0053BA80 (CMenuProfiles, menu_profiles)
// If an auxiliary object is stored at +0x24, invoke its first virtual method with the
// argument 1 (__thiscall, callee pops the stack arg) and then clear the stored pointer.
// Returns the callee's result; eax is left undefined when the member is null.
static int __fastcall CMenuProfiles_ReleaseSubObject(CMenuProfiles* self, void* /*edx*/)
{
    int result = 0;  // the original leaves eax undefined when there is nothing to release

    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x24)));
    if ( sub )
    {
        // vtable slot 0
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>((*reinterpret_cast<void***>(sub))[0])(sub, nullptr, 1);
        CSF_AT(uint32_t, self, 0x24) = 0;
    }
    return result;
}

HOOK(0x53BA80, CMenuProfiles_ReleaseSubObject, "auto");

