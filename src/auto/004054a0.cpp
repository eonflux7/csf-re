#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads the owner from field_0008, asks it (slot 10) for a target object, asks the
// target (slot 34) to map a2 to an index, and if that index is valid, returns the
// target's slot 32 result for that index. Returns 0 on any missing link.
static int __fastcall S_004054a0_ResolveChildAndDispatch(S_004054a0* self, void* /*edx*/, int a2) {
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));
    if (!owner) {
        return 0;
    }

    void* target = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    if (!target) {
        return 0;
    }

    int slotIndex = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, target, 0)[0x88 / 4])(target, nullptr, a2);
    if (slotIndex == -1) {
        return 0;
    }

    return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, target, 0)[0x80 / 4])(target, nullptr, slotIndex);
}

HOOK(0x004054A0, S_004054a0_ResolveChildAndDispatch, "auto");
