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

// Bound original callees and data by address.
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, MenuEula_GetPanelObject, 0x006683F0, (void* self, void* edx));
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);

// CMenuEula::HandleMessage (__thiscall, 2 stack args, ret 8).
// `self` is the this pointer as seen by the original (the owning object sits at self - 4).
static int __fastcall MenuEula_HandleMessage(void** self, void* /*edx*/, int msg, int param)
{
    const uintptr_t selfAddr = reinterpret_cast<uintptr_t>(self);

    if (msg == 1 || msg == 14)
    {
        // Clear the flag in the global state block.
        uint8_t* globalState = static_cast<uint8_t*>(GetGlobalState());
        globalState[0xD0] = 0;

        // Slot 19 (+0x4C) of the object at self - 4, no arguments.
        void* ownerObj = reinterpret_cast<void*>(selfAddr - 4);
        void** ownerVtbl = *reinterpret_cast<void***>(ownerObj);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(ownerVtbl[19])(ownerObj, nullptr);

        // Slot 22 (+0x58) of the child at self[4], passing the message string.
        void* childObj = *reinterpret_cast<void**>(selfAddr + 0x10);
        void** childVtbl = *reinterpret_cast<void***>(childObj);
        return reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(childVtbl[22])(
            childObj, nullptr, g_pszMenu_P_MePrin);
    }

    int result = msg - 50;
    if (msg == 50)
    {
        result = param;
        const uint8_t flag = *reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(param) + 56);
        if (flag != 0)
        {
            void* panel = MenuEula_GetPanelObject(self, nullptr);
            void** panelVtbl = *reinterpret_cast<void***>(panel);
            return reinterpret_cast<int(__fastcall*)(void*, void*)>(panelVtbl[7])(panel, nullptr);
        }
    }
    return result;
}

HOOK(0x00532B60, MenuEula_HandleMessage, "auto");
