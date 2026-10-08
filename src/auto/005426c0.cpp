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

// Original callees, bound by address.
// __thiscall with no stack arguments: ecx = this, pops 0 bytes (ret 0).
CSF_FUNC(int, __fastcall, MenuMpServerInfo_ActivateChild, 0x00530310, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
// Original passes the global state in ecx and one stack argument; callee pops 4 (ret 4).
CSF_FUNC(int, __fastcall, SetFlagAndRefresh, 0x0042DF80, (void* self, void* edx, int a1));
CSF_FUNC(void*, __cdecl, GetValue8B055C, 0x004F9660, ());

// Releases the pending child server-info panel (if any), resets the link,
// refreshes the global menu state and marks the value at +0xF8 as set.
static void __fastcall MenuMpCharacterSelect_ReleaseServerInfoChild(void* self, void* /*edx*/)
{
    void* childPanel = CSF_AT(void*, self, 0x4);
    if (childPanel)
    {
        MenuMpServerInfo_ActivateChild(childPanel, nullptr);
        CSF_AT(uint32_t, self, 0x4) = 0;

        void* globalState = GetGlobal8afc78();
        SetFlagAndRefresh(globalState, nullptr, 1);

        void* valueHolder = GetValue8B055C();
        CSF_AT(uint8_t, valueHolder, 0xF8) = 1;
    }
}

HOOK(0x005426C0, MenuMpCharacterSelect_ReleaseServerInfoChild, "auto");
