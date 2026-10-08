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

// Original callees (no-argument helpers, bound by address).
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x75FDF0, ());

// Tail path 1: ecx = this; mov eax,[ecx]; jmp [eax+0x4C] (vtable slot 19).
// Caller's stack (return address) is left untouched, as the original's pop esi + jmp does.
static __declspec(naked) int __fastcall TailJumpVtblSlot19(void* self, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x4c]
    }
}

// Tail path 2: ecx = child object; mov edx,[ecx]; jmp [edx+8] (vtable slot 2).
static __declspec(naked) int __fastcall TailJumpVtblSlot2(void* child, void* /*edx*/)
{
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x8]
    }
}

static int __fastcall MenuControls_DispatchTail(int* self, void* /*edx*/)
{
    GetGlobalValue_8B056C();

    // Notify the object at +0x20 through vtable slot 7 (+0x1C), passing &self[18] (+0x48).
    int notifyTarget = CSF_AT(int, self, 0x20);
    if (notifyTarget)
    {
        void** targetVtbl = CSF_AT(void**, notifyTarget, 0);
        ((void(__stdcall*)(void*))targetVtbl[7])(CSF_AT(void*, self, 0x48));
    }

    if (!CSF_AT(uint8_t, GetGlobalValue(), 0xCC))
    {
        return TailJumpVtblSlot19(self, nullptr);
    }

    int child = CSF_AT(int, self, 0x1C);
    if (child)
    {
        return TailJumpVtblSlot2(reinterpret_cast<void*>(child), nullptr);
    }
    return child;
}

HOOK(0x00542C40, MenuControls_DispatchTail, "auto");
