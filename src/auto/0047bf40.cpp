#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004DB140, __fastcall-style wrapper for a __thiscall method, 1 stack arg (ret 4).
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Sets the cover attack parameter block: stores the attack value into the inner
// task object, clears its counter, restarts movement on the inner task, and
// records the flag byte. Returns the flag byte. Callee pops 8 bytes (two stack args).
static uint8_t __fastcall C_0082f9f8_SetCoverAttackParameter(C_0082f9f8* self, void* /*edx*/, int a2, uint8_t a3) {
    // Sub-task at this+0x164 (pad_0020[324]); its StartMovement returns a pointer to the
    // object whose +0x08 holds the attack data block.
    C_0082f9f8* started = reinterpret_cast<C_0082f9f8*>(
        CharTask_StartMovement(self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x164))));

    CSF_AT(int, CSF_AT(uint8_t*, started, 0x08), 0x2B0) = a2;
    CSF_AT(int, CSF_AT(uint8_t*, started, 0x08), 0x2AC) = 0;

    CharTask_StartMovement(started, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(started) + 0x14)));

    CSF_AT(uint8_t, self, 0x2A8) = a3;
    CSF_AT(uint8_t, self, 0x2ED) = 1;
    return a3;
}

HOOK(0x0047BF40, C_0082f9f8_SetCoverAttackParameter, "auto");
