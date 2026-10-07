#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at [[self->field_000c + 0x2C] + 0xDC], with no null checks.
static int __fastcall C_0083e160_GetLinkedValueDC(C_0083e160* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c));
    void* inner = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x2c)));
    return CSF_AT(int, inner, 0xdc);
}

HOOK(0x004E1110, C_0083e160_GetLinkedValueDC, "auto");
