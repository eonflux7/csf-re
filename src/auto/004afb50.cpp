#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Clears the nine guard-AI state fields on the linked static-AI object.
static uint32_t* __stdcall AiFncStatic_ResetGuardState(int linkedArg) {
    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedArg)), nullptr);

    uint8_t* base = linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4))
        : nullptr;

    CSF_AT(int, base, 0x25C) = 0;
    CSF_AT(int, base, 0x2A8) = 0;
    CSF_AT(int, base, 0x2D8) = 0;
    CSF_AT(int, base, 0x2F8) = 0;
    CSF_AT(int, base, 0x314) = 0;
    CSF_AT(int, base, 0x330) = 0;
    CSF_AT(int, base, 0x368) = 0;
    CSF_AT(int, base, 0x38C) = 0;
    CSF_AT(int, base, 0x3B8) = 0;

    return reinterpret_cast<uint32_t*>(base);
}

HOOK(0x004AFB50, AiFncStatic_ResetGuardState, "auto");
