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

// Global declarations must live at file scope (CSF_GLOBAL expands to a declaration)
CSF_GLOBAL(int, g_globalValue_8B0560, 0x8B0560);
CSF_GLOBAL(unsigned int, g_fpControlWord_8BB3F4, 0x8BB3F4);

// Reads a global at 0x8B0560 (no args, cdecl-style)
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0560, 0x4FA6B0, ());
// Thiscall, no stack args; returns a sub-object pointer for this+4
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x6683F0, (void* self, void* edx));
// Thiscall, one stack arg, ret 4
CSF_FUNC(void, __fastcall, MenuControls_ApplyChildIndex, 0x712D90, (void* self, void* edx, int index));
CSF_FUNC(char, __cdecl, MenuControls_RefreshCursor, 0x4F9A20, ());
CSF_FUNC(void*, __cdecl, MenuControls_ClearSelectionState, 0x4FABB0, ());
CSF_FUNC(int, __cdecl, MenuControls_RefreshHighlight, 0x518A30, ());
CSF_FUNC(int, __cdecl, MenuControls_UpdateFooter, 0x518F90, ());
CSF_FUNC(int, __cdecl, MenuControls_UpdateScrollBars, 0x519020, ());
CSF_FUNC(int, __cdecl, MenuControls_UpdateEntries, 0x519090, ());
CSF_FUNC(int, __cdecl, MenuControls_UpdateTooltips, 0x519100, ());
// CRT _controlfp(newValue, mask), the callee of the 0x69D8C0 tail stub
CSF_FUNC(unsigned int, __cdecl, __controlfp, 0x7B070F, (unsigned int newValue, unsigned int mask));

static unsigned int __fastcall MenuControls_RefreshAndRestoreFpControl(char* self, void* /*edx*/)
{
    // Original reads the global into edi before calling GetGlobalValue_8B0560
    int savedGlobalValue = g_globalValue_8B0560;
    GetGlobalValue_8B0560();

    // Original pushes savedGlobalValue as a stack arg for ApplyChildIndex,
    // while ResolveChildHandle is called with ecx = self + 4 (ret 0).
    void* childHandle = GetReferencedObject(self + 4, nullptr);
    MenuControls_ApplyChildIndex(childHandle, nullptr, savedGlobalValue);

    MenuControls_RefreshHighlight();
    MenuControls_RefreshCursor();
    MenuControls_ClearSelectionState();
    MenuControls_UpdateEntries();
    MenuControls_UpdateScrollBars();
    MenuControls_UpdateTooltips();
    MenuControls_UpdateFooter();

    // Inlined body of the tail stub at 0x69D8C0:
    //   eax = [0x8BB3F4]; eax = _controlfp(eax, 0x300); [0x8BB3F4] = eax; return eax
    unsigned int newFpControlWord = __controlfp(g_fpControlWord_8BB3F4, 0x300u);
    g_fpControlWord_8BB3F4 = newFpControlWord;
    return newFpControlWord;
}

HOOK(0x543910, MenuControls_RefreshAndRestoreFpControl, "auto");
