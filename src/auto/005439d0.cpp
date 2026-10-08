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

// Original 0x005439D0 (CMenuControls).
// Stage 1 (C++, __cdecl): queries slot 0x74 of the menu object and returns that
// raw int unchanged. When its low byte is nonzero it also runs the refresh
// calls and stores the re-resolved object in g_dispatchObject.
// Stage 2 (naked thunk): on a nonzero low byte, tail-jump through slot 0x78 of
// g_dispatchObject with the caller's stack and registers untouched. Otherwise
// return the raw result in eax.

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_518A20, 0x00518A20, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0560, 0x004FA6B0, ());
CSF_FUNC(char, __fastcall, MenuControls_RefreshAOne, 0x004FAC20, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B055C, 0x004F9660, ());
CSF_FUNC(unsigned char, __fastcall, MenuControls_RefreshATwo, 0x004FA2D0, (void* self, void* edx));

// Object resolved after the refresh calls. Read by the thunk for the tail jump.
static void* g_dispatchObject = nullptr;

// Queries slot 0x74 and returns its raw eax. Runs the refresh path when the
// low byte is nonzero, and in that case leaves the final object in g_dispatchObject.
static int __cdecl MenuControls_QueryAndPrepareDispatch()
{
    int globalIndex = GetGlobalInt();
    void* menuObject = MenuControls_ResolveObject(reinterpret_cast<void*>(static_cast<uintptr_t>(globalIndex)), nullptr);

    // Slot 0x74 (index 29), thiscall, one int argument of 0.
    int queryResult = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, menuObject, 0)[29])(menuObject, nullptr, 0);

    if (static_cast<uint8_t>(queryResult) == 0)
    {
        return queryResult;
    }

    GetValue_518A20();
    int sourceValue = GetGlobalValue_8B0560();
    MenuControls_RefreshAOne(reinterpret_cast<void*>(static_cast<uintptr_t>(sourceValue)), nullptr);
    int secondValue = GetValue8B055C();
    MenuControls_RefreshATwo(reinterpret_cast<void*>(static_cast<uintptr_t>(secondValue)), nullptr);

    globalIndex = GetGlobalInt();
    g_dispatchObject = MenuControls_ResolveObject(reinterpret_cast<void*>(static_cast<uintptr_t>(globalIndex)), nullptr);
    return queryResult;
}

// Entry point. Calls the C++ stage, then either returns its raw result or
// tail-jumps through slot 0x78. The stack is back to its entry layout after the
// call, so the jmp forwards the caller's arguments.
static __declspec(naked) void CMenuControls_RefreshAndDispatch()
{
    __asm {
        lea eax, MenuControls_QueryAndPrepareDispatch
        call eax
        test al, al
        je ReturnRawResult
        mov ecx, dword ptr [g_dispatchObject]
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x78]
ReturnRawResult:
        ret
    }
}

HOOK(0x005439D0, CMenuControls_RefreshAndDispatch, "auto");
