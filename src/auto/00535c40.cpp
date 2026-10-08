#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// GetGlobalState_50D190(): returns pointer to global state block (__cdecl, no args).
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// GetGlobalInt(): returns global integer value (__cdecl, no args).
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

// Intro-video state advance (__thiscall, one float stack arg, ret 4).
CSF_FUNC(void, __fastcall, CMenuIntroVideo_AdvanceTimerAndFinish, 0x005355E0, (void* self, void* edx, float a2));

// Begins intro video playback (__thiscall, no stack args, ret 0). Semantics not fully known.
CSF_FUNC(void*, __fastcall, CMenuIntroVideo_RefreshState, 0x00535900, (void* self, void* edx));

// Predicate over the global int gate (__thiscall, ecx = global int value, ret 0). Returns byte in al.
CSF_FUNC(uint8_t, __fastcall, IsGlobalIntGateOpen, 0x006B1680, (void* self, void* edx));

static void __fastcall CMenuIntroVideo_Tick(CMenuIntroVideo* self, void* /*edx*/, float a2)
{
    // Notify global state object (vtable slot 92) with code 11 when present.
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    uint32_t stateObj = CSF_AT(uint32_t, globalState, 0x10);
    if (stateObj != 0)
    {
        void* obj = reinterpret_cast<void*>(stateObj);
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, obj, 0)[92 / 4]))(obj, nullptr, 11);
    }

    uint32_t playbackPhase = self->field_0014;
    if (playbackPhase == 0)
    {
        int globalIntValue = GetGlobalInt();
        if (IsGlobalIntGateOpen(reinterpret_cast<void*>(static_cast<uintptr_t>(globalIntValue)), nullptr))
        {
            CMenuIntroVideo_RefreshState(self, nullptr);
        }
    }
    else if (playbackPhase == 1)
    {
        CMenuIntroVideo_AdvanceTimerAndFinish(self, nullptr, a2);
    }
}

HOOK(0x00535C40, CMenuIntroVideo_Tick, "auto");
