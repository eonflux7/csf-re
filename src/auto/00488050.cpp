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

// Callee bindings
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_Unknown_InvokeSubobjectVfunc2C, 0x004F51A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveAttachedSubobject, 0x00478280, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharActionFsm_ResetPendingTarget, 0x004847C0, (void* self, void* edx));
CSF_FUNC(uint8_t, __fastcall, CharActionFsm_HandleEventE, 0x00484B20, (CCharActionFsm* self, void* edx, int a3));
CSF_FUNC(char, __fastcall, CharActionFsm_HandleEvent13, 0x00485570, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, CharActionFsm_PrepareReload, 0x00486460, (void* self, void* edx));

// Virtual slot signatures (__thiscall, passed as __fastcall with edx unused)
using VirtNoArgInt   = int(__fastcall*)(void* self, void* edx);
using VirtNoArgPtr   = void*(__fastcall*)(void* self, void* edx);
using VirtNoArgVoid  = void(__fastcall*)(void* self, void* edx);
using VirtOneArgInt  = int(__fastcall*)(void* self, void* edx, int a);
using VirtTwoArgInt  = int(__fastcall*)(void* self, void* edx, int a, int b);
using VirtOneArgVoid = void(__fastcall*)(void* self, void* edx, int a);

// Dispatches a character action FSM state (self->state at +0x10, active flag at +0x69).
// Returns the low byte of the last sub-call's result, as the original does via al.
static char __fastcall CharActionFsm_DispatchStateCase(CCharActionFsm* self, void* /*edx*/, int a3)
{
    const uint32_t stateCode = CSF_AT(uint32_t, self, 0x10);
    // Default result when no case matches: the state minus 12 (left in al by the original).
    char result = static_cast<char>(stateCode - 12u);

    switch (stateCode)
    {
    case 0xC:
    {
        result = static_cast<char>(CSF_AT(uint8_t, self, 0x69));
        if (result)
        {
            void* weaponObj = CSF_AT(void*, self, 0x9C);
            if (weaponObj != nullptr &&
                reinterpret_cast<VirtNoArgInt>(CSF_AT(void**, weaponObj, 0)[19])(weaponObj, nullptr) == 12)
            {
                result = static_cast<char>(
                    reinterpret_cast<VirtOneArgInt>(CSF_AT(void**, self, 0)[78])(self, nullptr, 13));
            }
            else
            {
                void* subobj = CSF_AT(void*, self, 0xA4);
                if (reinterpret_cast<VirtNoArgInt>(CSF_AT(void**, subobj, 0)[19])(subobj, nullptr) == 3)
                {
                    result = static_cast<char>(
                        reinterpret_cast<VirtOneArgInt>(CSF_AT(void**, self, 0)[78])(self, nullptr, 17));
                }
                else
                {
                    result = static_cast<char>(
                        reinterpret_cast<VirtOneArgInt>(CSF_AT(void**, self, 0)[78])(self, nullptr, 14));
                }
            }
        }
        break;
    }
    case 0xD:
    {
        const int globalValue = GetGlobalValue_4FBDE0();
        const int firstElement = GetFirstElement(
            reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr);
        result = static_cast<char>(reinterpret_cast<uintptr_t>(provider));

        if (CSF_AT(uint8_t, self, 0x69))
        {
            result = static_cast<char>(C_Unknown_InvokeSubobjectVfunc2C(provider, nullptr));
            if (!result)
            {
                CharActionFsm_ResetPendingTarget(self, nullptr);
                if (CSF_AT(void*, self, 0xA0) != nullptr)
                {
                    result = static_cast<char>(
                        reinterpret_cast<VirtOneArgInt>(CSF_AT(void**, self, 0)[78])(self, nullptr, 16));
                }
                else
                {
                    result = static_cast<char>(
                        reinterpret_cast<VirtTwoArgInt>(CSF_AT(void**, self, 0)[77])(self, nullptr, 2, 0));
                }
            }
        }
        break;
    }
    case 0xE:
        return static_cast<char>(CharActionFsm_HandleEventE(self, nullptr, a3));
    case 0xF:
    {
        result = static_cast<char>(CSF_AT(uint8_t, self, 0x69));
        if (result)
        {
            void* weaponObj = CSF_AT(void*, self, 0x9C);
            if (weaponObj != nullptr)
            {
                CSF_AT(void*, self, 0xA0) = weaponObj;
                CSF_AT(void*, self, 0x9C) = nullptr;
                reinterpret_cast<VirtOneArgVoid>(CSF_AT(void**, weaponObj, 0)[12])(weaponObj, nullptr, 0);
            }
            result = static_cast<char>(
                reinterpret_cast<VirtOneArgInt>(CSF_AT(void**, self, 0)[78])(self, nullptr, 14));
        }
        break;
    }
    case 0x10:
    case 0x16:
        result = static_cast<char>(CSF_AT(uint8_t, self, 0x69));
        if (result)
            result = static_cast<char>(
                reinterpret_cast<VirtTwoArgInt>(CSF_AT(void**, self, 0)[77])(self, nullptr, 0, 0));
        break;
    case 0x11:
    {
        result = static_cast<char>(CSF_AT(uint8_t, self, 0x69));
        if (result)
        {
            void* subobj = CSF_AT(void*, self, 0xA4);
            void* found = reinterpret_cast<void*>(ResolveAttachedSubobject(subobj, nullptr));
            if (found != nullptr)
            {
                void* v9 = reinterpret_cast<VirtNoArgPtr>(CSF_AT(void**, found, 0)[10])(found, nullptr);
                reinterpret_cast<VirtNoArgVoid>(CSF_AT(void**, v9, 0)[41])(v9, nullptr);
            }
            result = static_cast<char>(
                reinterpret_cast<VirtOneArgInt>(CSF_AT(void**, self, 0)[78])(self, nullptr, 18));
        }
        break;
    }
    case 0x12:
        result = static_cast<char>(CSF_AT(uint8_t, self, 0x69));
        if (result)
        {
            CharActionFsm_PrepareReload(self, nullptr);
            result = static_cast<char>(
                reinterpret_cast<VirtTwoArgInt>(CSF_AT(void**, self, 0)[77])(self, nullptr, 2, 0));
        }
        break;
    case 0x13:
        return CharActionFsm_HandleEvent13(self, nullptr, a3);
    case 0x14:
        result = static_cast<char>(CSF_AT(uint8_t, self, 0x69));
        if (result)
            result = static_cast<char>(
                reinterpret_cast<VirtOneArgInt>(CSF_AT(void**, self, 0)[78])(self, nullptr, 21));
        break;
    case 0x15:
        result = static_cast<char>(CSF_AT(uint8_t, self, 0x69));
        if (result)
            result = static_cast<char>(
                reinterpret_cast<VirtTwoArgInt>(CSF_AT(void**, self, 0)[77])(self, nullptr, 0, 0));
        break;
    default:
        break;
    }
    return result;
}

HOOK(0x00488050, CharActionFsm_DispatchStateCase, "auto");
