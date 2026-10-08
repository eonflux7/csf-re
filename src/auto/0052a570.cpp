#include <cstdint>
#include "globals.h"
#include "hook.h"

// Parent-object handler invoked when exactly one pending action flag is set.
// Original: 0x528E70, __thiscall, no stack args (ret 0).
CSF_FUNC(int, __fastcall, CMenuMessageBox_MarkCurrentButtonFlag, 0x528E70, (void* self, void* edx));

// Clears the pending-flag dirty byte at +0x7D and, when the pending flag word
// (+0x78) is exactly one of the single bits 0x1..0x400, dispatches to the
// parent-object handler at (this - 4). Returns the low byte of the result,
// or the low byte of the flag word when no dispatch happens.
static char __fastcall MenuMessageBox_ProcessSingleFlagCommand(uint8_t* self, void* /*edx*/, int /*a2*/)
{
    // Nothing pending: original returns al = byte at +0x7D, which is 0.
    if (CSF_AT(uint8_t, self, 0x7D) == 0)
        return 0;

    // Read the flag word before clearing the dirty byte (as the original does).
    const uint32_t pendingFlags = CSF_AT(uint32_t, self, 0x78);
    CSF_AT(uint8_t, self, 0x7D) = 0;

    // Dispatch only when exactly one of the bits 0x1 .. 0x400 is set and nothing else.
    for (uint32_t singleBit = 0x1; singleBit <= 0x400; singleBit <<= 1)
    {
        if (pendingFlags == singleBit)
        {
            return static_cast<char>(CMenuMessageBox_MarkCurrentButtonFlag(self - 4, nullptr));
        }
    }

    // No dispatch: al still holds the low byte of the flag word.
    return static_cast<char>(pendingFlags & 0xFF);
}

HOOK(0x52A570, MenuMessageBox_ProcessSingleFlagCommand, "auto");
