#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: 0x48E460, __fastcall-style binding (ecx = self, one stack arg).
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

// Dispatches through two returned objects' vtables after configuring the focus entry.
// Original: 0x49D060, __thiscall with two stack args (ret 8).
static int __fastcall C_00834050_ConfigureFocusThenDispatch(C_00834050* self, void* /*edx*/, int* a2, int a3) {
    SceneLoader_ConfigureFocusEntry(self, nullptr, a2);

    // First object comes from the typed virtual getter (slot +0x28).
    void* focusObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vGetField016c()));

    // Second object: focusObject->vtbl[+0x18] (slot 6), called with no arguments.
    void* dispatchObject = reinterpret_cast<void*>(static_cast<uintptr_t>(
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, focusObject, 0)[6])(focusObject, nullptr)));

    // Final call: dispatchObject->vtbl[+0x4C] (slot 19) with a3.
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, dispatchObject, 0)[19])(dispatchObject, nullptr, a3);
}

HOOK(0x0049D060, C_00834050_ConfigureFocusThenDispatch, "auto");
