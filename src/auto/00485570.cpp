#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall char CharActionFsm::NotifyChildAndSetState13IfFlagged(int) (ret 4)
// If the flag byte at +0x69 is set, passes a 3-dword notify record (vtable C_0082d448,
// -1, -1) to the child at +0x9C (vtable slot 17), then dispatches state 13 through
// this object's vtable slot 78. Otherwise returns the flag byte (0).
static char __fastcall CharActionFsm_NotifyChildAndSetState13IfFlagged(void* self, void* /*edx*/, int /*a2*/)
{
    char flagResult = CSF_AT(char, self, 0x69);
    if (flagResult) {
        void* childObject = CSF_AT(void*, self, 0x9C);

        uint32_t notifyData[3];
        notifyData[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        notifyData[1] = 0xFFFFFFFFu;
        notifyData[2] = 0xFFFFFFFFu;
        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, childObject, 0)[17])(childObject, nullptr, notifyData);

        return ((char(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[78])(self, nullptr, 13);
    }
    return flagResult;
}

HOOK(0x485570, CharActionFsm_NotifyChildAndSetState13IfFlagged, "auto");
