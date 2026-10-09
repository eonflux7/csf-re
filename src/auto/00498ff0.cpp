#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: sub_496CD0 (__thiscall, one stack arg, ret 4).
CSF_FUNC(void, __fastcall, SubmitMovementRequest, 0x00496CD0, (void* self, void* edx, int a2));

// Sets up a 0x24-byte movement request record on the stack and submits it
// to the character's movement queue, unless the character's flag 0x10 is set.
static void __fastcall Character_RequestMovementAction(C_00833b88* self, void* /*edx*/)
{
    if ((CSF_AT(uint8_t, self, 0x2E4) & 0x10) != 0)
        return;

    // Request record, laid out at [ebp-0x24] in the original frame.
    uint8_t request[0x24];

    CSF_AT(uint32_t, request, 0x00) = 1;                       // request kind
    CSF_AT(uint32_t, request, 0x04) = 0;
    CSF_AT(void*, request, 0x08) = CSF_VTBL(0x0082D448);      // table pointer stored in the record
    CSF_AT(int32_t, request, 0x0C) = -1;
    CSF_AT(int32_t, request, 0x10) = -1;
    CSF_AT(uint32_t, request, 0x14) = 0;                       // 12 bytes zeroed
    CSF_AT(uint32_t, request, 0x18) = 0;
    CSF_AT(uint32_t, request, 0x1C) = 0;
    CSF_AT(uint8_t, request, 0x20) = 1;                        // flags
    CSF_AT(uint8_t, request, 0x21) = 1;
    CSF_AT(uint8_t, request, 0x22) = 1;

    SubmitMovementRequest(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(request)));
}

HOOK(0x00498FF0, Character_RequestMovementAction, "auto");
