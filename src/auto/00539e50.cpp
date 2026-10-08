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

// Returns the global object pointer at 0x4FBDE0 (__cdecl, no args; caller does not clean up).
CSF_FUNC(void*, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());

// __thiscall accessor at 0x4FC0A0; takes the global object and returns a sub-object pointer.
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));

// __thiscall predicate at 0x443230; one stack arg, callee pops 4 bytes; returns bool in al.
CSF_FUNC(bool, __fastcall, IsPendingIndexMatch_443230, 0x00443230, (void* self, void* edx, int pendingIndex));

// __userpurge at 0x4FCEB0: ecx = object, edi = a2 (not representable here), one stack arg, ret 4.
CSF_FUNC(char, __fastcall, ForwardToGlobalHandler_4FCEB0, 0x004FCEB0, (void* self, void* edx, int a3));

// Clears the menu's pending selection index and invokes the virtual reset slot (+0x58).
static char __fastcall MenuMain_ClearPendingIndexAndForward(int* self, void* /*edx*/, int a3)
{
    int pendingIndex = CSF_AT(int, self, 0x44);

    void* globalObject = GetGlobalValue_4FBDE0();
    void* subObject = GetGlobalSubObject_4FC0A0(globalObject, nullptr);

    if (IsPendingIndexMatch_443230(subObject, nullptr, pendingIndex))
    {
        void** vtbl = CSF_AT(void**, self, 0);
        CSF_AT(int, self, 0x14) = -1;
        CSF_AT(int, self, 0x44) = -1;
        ((void(__fastcall*)(void*, void*))vtbl[0x58 / 4])(self, nullptr);
    }

    void* globalObjectAgain = GetGlobalValue_4FBDE0();
    return ForwardToGlobalHandler_4FCEB0(globalObjectAgain, reinterpret_cast<void*>(static_cast<intptr_t>(0)), a3);
}

HOOK(0x00539E50, MenuMain_ClearPendingIndexAndForward, "auto");
