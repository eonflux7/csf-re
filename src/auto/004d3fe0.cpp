#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees (bound by address, original signatures).
CSF_FUNC(bool, __fastcall, C_0083bc28_CanEnterPhase, 0x004D36C0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0083bc28_ApplyPhase, 0x004D3E00, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));

// Per-frame state update for C_0083bc28: releases the pending BDD handle when the
// phase is 5 and the flag bit 0x10 is clear, clears the 0x8 flag, or advances to a
// new phase (3 or 4) when the transition is allowed.
static void __fastcall C_0083bc28_UpdatePendingHandleState(C_0083bc28* self, void* /*edx*/)
{
    uint32_t flags = self->field_0044;

    if (self->field_004c == 5)
    {
        if ((flags & 0x10) == 0)
        {
            int globalState = GetGlobalState_50D190();
            BddLoader_ReleasePendingHandle(
                reinterpret_cast<void*>(static_cast<std::intptr_t>(globalState)), nullptr);
            self->field_004c = 2;
        }
    }
    else if ((flags & 8) != 0)
    {
        self->field_0044 = flags & 0xFFFFFFF7u;
    }
    else
    {
        int phase = ((flags & 2) != 0) + 3;
        if (C_0083bc28_CanEnterPhase(self, nullptr, phase))
        {
            C_0083bc28_ApplyPhase(self, nullptr, phase);
            self->field_004c = static_cast<uint32_t>(phase);
        }
    }
}

HOOK(0x004D3FE0, C_0083bc28_UpdatePendingHandleState, "auto");
