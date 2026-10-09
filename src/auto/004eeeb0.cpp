#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_4EEB90, __thiscall with two stack args (ret 8).
CSF_FUNC(void*, __fastcall, Scene_InitObjectSlot, 0x004EEB90, (void* self, void* edx, void* a3, int arg4));

// Virtual lookup on the object held in this->field_1fc8: vtable slot 16 / 4 = 4.
// Returns -1 on failure.
static C_00833908* __fastcall Scene_ResolveObjectAndInit(S_004e9090* self, void* /*edx*/, int a2, int a3)
{
    void* lookupObject = reinterpret_cast<void*>(self->field_1fc8);
    using VirtualLookupFn = int(__fastcall*)(void* self, void* edx, int index);
    VirtualLookupFn lookupFn = reinterpret_cast<VirtualLookupFn>(CSF_AT(void**, lookupObject, 0)[4]);
    int foundIndex = lookupFn(lookupObject, nullptr, a2);

    if (foundIndex == -1)
        return nullptr;

    C_00833908* initialized = reinterpret_cast<C_00833908*>(
        Scene_InitObjectSlot(self, nullptr, reinterpret_cast<void*>(foundIndex), a3));
    return initialized;
}

HOOK(0x004EEEB0, Scene_ResolveObjectAndInit, "auto");
