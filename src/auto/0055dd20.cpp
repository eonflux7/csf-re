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

// Callee: global state accessor (__cdecl, no args).
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x006A6D70, ());

// Data referenced by the original.
CSF_GLOBAL(const char*, g_pszMenu_P_MeJuMP, 0x00899AF8);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFrIn, 0x00899B5C);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(std::uint8_t, g_bMenuFlag_8B0902, 0x008B0902);

// Virtual call signatures (vtable slots = byte offset / 4).
using VirtNoArgRet = void* (__fastcall*)(void* self, void* edx);
using VirtIntArgVoid = void (__fastcall*)(void* self, void* edx, int arg);
using VirtPtrArgRet = int (__fastcall*)(void* self, void* edx, const void* arg);
using VirtListRefreshVoid = void (__fastcall*)(void* self, void* edx, CMenuPlayerList* menu, int arg);

static int __fastcall CMenuPlayerList_ResetAndShowDefaultLabel(CMenuPlayerList* self, void* /*edx*/)
{
    // Optional: if global state exists and its child reports true, clear that child's state.
    if (GetGlobalState())
    {
        void* stateA = GetGlobalState();
        if (reinterpret_cast<VirtNoArgRet>(CSF_AT(void**, stateA, 0)[0xF4 / 4])(stateA, nullptr))
        {
            void* stateB = GetGlobalState();
            void* child = reinterpret_cast<VirtNoArgRet>(CSF_AT(void**, stateB, 0)[0xF4 / 4])(stateB, nullptr);
            reinterpret_cast<VirtIntArgVoid>(CSF_AT(void**, child, 0)[0x18 / 4])(child, nullptr, 0);
        }
    }

    // Refresh the list object (field +0x14) with self as argument.
    void* list = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<VirtListRefreshVoid>(CSF_AT(void**, list, 0)[0x60 / 4])(list, nullptr, self, 0);

    // If a result is already set, return it directly.
    if (self->field_0018 != 0)
    {
        return static_cast<int>(self->field_0018);
    }

    // Pick the label: online/LbFrIn variant when flags are set, otherwise MeJuMP.
    const char* label = g_pszMenu_P_MeJuMP;
    if (g_bMenuFlag_8B0902 && g_OnlineMatchFlag)
    {
        label = g_pszMenu_P_LbFrIn;
    }

    // Reload the list object (it may have changed during the call above).
    void* listAgain = reinterpret_cast<void*>(self->field_0014);
    return reinterpret_cast<VirtPtrArgRet>(CSF_AT(void**, listAgain, 0)[0x58 / 4])(listAgain, nullptr, label);
}

HOOK(0x0055DD20, CMenuPlayerList_ResetAndShowDefaultLabel, "auto");
