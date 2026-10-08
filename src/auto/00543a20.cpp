#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0560, 0x004FA6B0, (void));
CSF_FUNC(char, __fastcall, GlobalMenu_ApplyFlagReset, 0x004FB5F0, (int self, void* edx, char a2));
CSF_FUNC(char, __fastcall, MenuControls_ForwardInner, 0x00535E60, (int** self, void* edx, int a2));

static char __fastcall MenuControls_ForwardAfterOptionalReset(int** self, void* /*edx*/, int a2)
{
    int globalObject = GetGlobalValue_8B0560();
    if (*reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(globalObject) + 0x10) != 0)
    {
        int refreshedObject = GetGlobalValue_8B0560();
        GlobalMenu_ApplyFlagReset(refreshedObject, nullptr, 0);
    }
    return MenuControls_ForwardInner(self, nullptr, a2);
}

HOOK(0x00543A20, MenuControls_ForwardAfterOptionalReset, "auto");
