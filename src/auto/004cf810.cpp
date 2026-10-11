#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets or clears bit 0 of the flags word (+0x40). A nonzero a2 sets the bit and
// stores 3 in +0x44; zero clears the bit and stores 0. Before that, it calls
// vtable slot 10 (+0x28) on the child object at +0x14. The result is the new flags word.
static unsigned int __fastcall C_0083b4a0_SetActiveFlagAndState(C_0083b4a0* self, void* /*edx*/, char a2)
{
    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x28 / 4])(child, nullptr);

    unsigned int result = self->field_0040;
    if (a2) {
        result = result | 1u;
        self->field_0044 = 3;
    } else {
        result = result & 0xFFFFFFFEu;
        self->field_0044 = 0;
    }
    self->field_0040 = result;
    return result;
}

HOOK(0x4CF810, C_0083b4a0_SetActiveFlagAndState, "auto");
