#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee: TCharTask-derived movement starter (__thiscall, ret 4).
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140,
         (C_0082f9f8* self, void* /*edx*/, int movement));

// Callee: returns bool (test al, al in the original). Takes the owner object as `this`.
CSF_FUNC(bool, __fastcall, sub_4D4C80, 0x004D4C80, (void* self, void* /*edx*/));

// Picks the movement target for this task and starts it.
// The owner's byte at +0x3E4 selects the first movement slot (+0x14).
// Otherwise the secondary check picks +0x3C or +0x28.
static int __fastcall C_0082f9f8_ChooseMovementAndStart(C_0082f9f8* self, void* /*edx*/) {
    void* owner = reinterpret_cast<void*>(self->field_0008);

    if (CSF_AT(uint8_t, owner, 0x3E4)) {
        return C_0082f9f8_StartMovement_4DB140(
            self, nullptr, reinterpret_cast<int>(&self->field_0014));
    }

    if (sub_4D4C80(owner, nullptr)) {
        return C_0082f9f8_StartMovement_4DB140(
            self, nullptr, reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x3C));
    }

    return C_0082f9f8_StartMovement_4DB140(
        self, nullptr, reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x28));
}

HOOK(0x004D4F70, C_0082f9f8_ChooseMovementAndStart, "auto");
