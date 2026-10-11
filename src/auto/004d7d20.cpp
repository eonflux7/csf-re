#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Clears bit 0 and sets bit 1 in the 32-bit flags word at +0x18 of the object
// referenced by self->field_0008. Returns a pointer to that flags word.
static int* __fastcall UpdateStateFlagsOfLinkedObject(void* self, void* /*edx*/) {
    uint8_t* linked = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    CSF_AT(uint32_t, linked, 0x18) &= ~1u;

    uint8_t* linkedAgain = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    CSF_AT(uint32_t, linkedAgain, 0x18) = CSF_AT(uint32_t, linkedAgain, 0x18) | 2u;

    return reinterpret_cast<int*>(linkedAgain + 0x18);
}

HOOK(0x4D7D20, UpdateStateFlagsOfLinkedObject, "auto");
