#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup (C_008629a4 part of the object).
CSF_FUNC(void, __fastcall, C_008629a4_Destroy, 0x006F8FF0, (C_008629a4* self, void* edx));
// Per-class update/teardown step of C_00862b04 (its result is not used by the original).
CSF_FUNC(char, __fastcall, C_00862b04_Update, 0x006F9820, (C_00862b04* self, void* edx));
// Game heap free.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown of C_00862b04. Sets the vtables, runs the update
// step and base cleanup, and frees the block when the low bit of a2 is set.
static int* __fastcall C_00862b04_Destroy(C_00862b04* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00862B04);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00862AE8));

    C_00862b04_Update(self, nullptr);
    C_008629a4_Destroy(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x6F9BF0, C_00862b04_Destroy, "auto");
