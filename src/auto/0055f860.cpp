#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, GetGlobalState, 0x50D190, ());
CSF_FUNC(int, __fastcall, Object_IsFieldF0NonZero, 0x50E210, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalState_ResetIfFlagged, 0x50E1E0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GlobalState_ResetWithTag, 0x50F790,
         (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_GLOBAL(int, g_MpSessionState, 0x8B0690);

static uint32_t __fastcall MenuMpOnline_ResetSession(void* self, void* /*edx*/)
{
    // If the global state reports field F0 as set, reset that state object first.
    if ((Object_IsFieldF0NonZero(GetGlobalState(), nullptr) & 0xFF) != 0)
    {
        GlobalState_ResetIfFlagged(GetGlobalState(), nullptr);
    }

    // Virtual call on the sub-object stored at +0x3C: slot 0x64 / 4 = 25, args (0, 0).
    void* subObject = CSF_AT(void*, self, 0x3C);
    ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, subObject, 0)[25])(subObject, nullptr, 0, 0);

    // Virtual call on self: slot 0x54 / 4 = 21, no args.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[21])(self, nullptr);

    g_MpSessionState = 0;

    return GlobalState_ResetWithTag(GetGlobalState(), nullptr, 0x494C3237, 0x20, 1, 0);
}

HOOK(0x55F860, MenuMpOnline_ResetSession, "auto");
