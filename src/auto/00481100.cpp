#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls slot 13 (+0x34) of every element in the child-state vector
// [field_0018, field_001c) with the same argument, and returns the last
// callee's result (or field_0018 itself when the vector is empty).
static int __fastcall CCharWeaponFsm_BroadcastToChildStates(CCharWeaponFsm* self, void* /*edx*/, int a2) {
    int result = (int)self->field_0018;

    unsigned int childCount = 0;
    if (self->field_0018 != 0) {
        childCount = (unsigned int)((int)(self->field_001c - self->field_0018) >> 2);
    }

    for (unsigned int index = 0; index < childCount; ++index) {
        // The element array is re-read from the object on every iteration.
        uint32_t childValue = reinterpret_cast<uint32_t*>(self->field_0018)[index];
        void* child = reinterpret_cast<void*>(childValue);

        auto childMethod = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, child, 0)[52 / 4]);
        result = childMethod(child, nullptr, a2);
    }

    return result;
}

HOOK(0x481100, CCharWeaponFsm_BroadcastToChildStates, "auto");
