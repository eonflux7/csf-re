#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Scene lookup by key. Callee is __thiscall-style: the original leaves ecx untouched,
// so the callee receives this as ecx and a2 as its single stack argument (ret 4).
CSF_FUNC(int, __fastcall, Scene_LookupEntryByKey, 0x00468750, (void* self, void* edx, int a2));

// The original references S_00468750's vtable through the address of its own call target.
// This is a compile-time reference only; it does not execute at runtime.
[[maybe_unused]] static const void* const kSceneKeyStructVtbl = CSF_VTBL(0x00468750);

// Returns true when the key is absent from the scene, or the found entry's field at +0x18 is zero.
static bool __fastcall MissionScript_KeyMissingOrHasNoValue(S_00468750* self, void* /*edx*/, int a2)
{
    int entryAddress = Scene_LookupEntryByKey(self, nullptr, a2);
    if (entryAddress == 0)
    {
        return true;
    }

    void* entry = reinterpret_cast<void*>(static_cast<std::uintptr_t>(entryAddress));
    int entryValueAt18 = CSF_AT(int, entry, 0x18);
    return entryValueAt18 == 0;
}

HOOK(0x004687B0, MissionScript_KeyMissingOrHasNoValue, "auto");
