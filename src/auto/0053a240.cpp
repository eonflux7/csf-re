#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x00521BB0: int __cdecl GetGlobalValue(void); returns a global object pointer.
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, (void));

// 0x00521F80: int(void); called with ecx = previous result, so bound as fastcall with one register arg.
CSF_FUNC(int, __fastcall, ResetSharedMenuState, 0x521F80, (int ecxValue, void* edx));

// 0x005233D0: void __thiscall(int this); bound with ecx = previous result.
CSF_FUNC(void, __fastcall, SetActiveScreenRef, 0x5233D0, (int ecxValue, void* edx));

// 0x004FDBB0: int __cdecl(void); returns a second global pointer used as the table owner.
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x4FDBB0, (void));

// 0x00501E50: std::_String_base* __thiscall(unsigned int* this); result is unused by the caller.
CSF_FUNC(int, __fastcall, ReleaseMenuStringTable, 0x501E50, (int ecxValue, void* edx));

// 0x00539D10: int __thiscall(this, int a2); pops 4 bytes (ret 4).
CSF_FUNC(int, __fastcall, CMenuScreen_Enter, 0x539D10, (void* self, void* edx, int a2));

static int __fastcall CMenuPause_CleanupAndEnterScreen(void* self, void* /*edx*/)
{
    ResetSharedMenuState(GetGlobalValue(), nullptr);
    SetActiveScreenRef(GetGlobalValue(), nullptr);
    ReleaseMenuStringTable(GetGlobalValue_8B0568(), nullptr);
    return CMenuScreen_Enter(self, nullptr, 0);
}

HOOK(0x0053A240, CMenuPause_CleanupAndEnterScreen, "auto");
