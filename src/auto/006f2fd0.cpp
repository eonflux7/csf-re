#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Normalizes the 3-component vector stored at the sub-object (this + 0x28).
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

// Copies a 3-dword vector from a3 into the embedded vector at this+0x28 (second
// component zeroed), normalizes it in place, then dispatches virtual slot 0x38
// with the caller's first argument.
static int __fastcall C_00862640_SetDirectionVectorAndNotify(C_00862640* self, void* /*edx*/, int a2, uint32_t* a3)
{
    uint32_t componentY = a3[1];
    uint32_t componentX = a3[0];

    CSF_AT(uint32_t, self, 0x28) = componentX;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = componentY;

    NormalizeVector3(reinterpret_cast<C_00833b88*>(reinterpret_cast<uint8_t*>(self) + 0x28), nullptr);

    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x38 / 4])(self, nullptr, a2);
}

HOOK(0x006F2FD0, C_00862640_SetDirectionVectorAndNotify, "auto");
