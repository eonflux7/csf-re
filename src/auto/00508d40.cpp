#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuCommon_SetFlagAndNotifyChild, 0x0050D7C0, (void* self, void* , int value));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(void*, g_pMenuManager, 0x008B0570);
CSF_GLOBAL(int, g_Value_8BB450, 0x008BB450);

using PostQuitMessageFunction = void (__stdcall*)(int);
using BeginPaintFunction = void* (__stdcall*)(void*, void*);
using EndPaintFunction = int (__stdcall*)(void*, const void*);
using DefWindowProcFunction = int (__stdcall*)(void*, uint32_t, uint32_t, int32_t);

CSF_GLOBAL(PostQuitMessageFunction, g_PostQuitMessage, 0x0082D2A8);
CSF_GLOBAL(EndPaintFunction, g_EndPaint, 0x0082D2AC);
CSF_GLOBAL(BeginPaintFunction, g_BeginPaint, 0x0082D2B0);
CSF_GLOBAL(DefWindowProcFunction, g_DefWindowProcA, 0x0082D2B4);

static int __stdcall AppMain_WindowProcedure(
    void* window, uint32_t message, uint32_t wParam, int32_t lParam)
{
    switch (message)
    {
    case 2:
        g_PostQuitMessage(0);
        return 0;

    case 6:
        if (!g_pMenuManager)
            return 0;

        if ((wParam & 0xFFFF) != 0 && (wParam >> 16) == 0)
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, state, 0)[0x10 / 4])(state, nullptr, 1);

            state = reinterpret_cast<void*>(GetGlobalState_50D190());
            MenuCommon_SetFlagAndNotifyChild(state, nullptr, 1);
        }
        else
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());

            // The original replaces only the low byte of lParam.
            const int32_t activationValue = static_cast<int32_t>(
                (static_cast<uint32_t>(lParam) & 0xFFFFFF00u) |
                static_cast<uint32_t>(lParam != 0));

            ((void (__fastcall*)(void*, void*, int32_t))
                CSF_AT(void**, state, 0)[0x10 / 4])(
                    state, nullptr, activationValue);

            state = reinterpret_cast<void*>(GetGlobalState_50D190());
            MenuCommon_SetFlagAndNotifyChild(state, nullptr, 0);
        }
        return 0;

    case 0xF:
    {
        alignas(8) uint8_t paintBytes[0x40];
        g_BeginPaint(window, paintBytes);
        g_EndPaint(window, paintBytes);
        return 0;
    }

    case 0x14:
        if (!g_Value_8BB450)
            return g_DefWindowProcA(window, message, wParam, lParam);
        return 1;

    case 28:
        if (!wParam && g_pMenuManager)
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, state, 0)[0x10 / 4])(state, nullptr, 0);

            state = reinterpret_cast<void*>(GetGlobalState_50D190());
            MenuCommon_SetFlagAndNotifyChild(state, nullptr, 0);
        }
        return 0;

    case 33:
        return 2;

    case 133:
        if (g_Value_8BB450)
        {
            void* registry = reinterpret_cast<void*>(GetGlobalInt());
            if (LookupObjectById(registry, nullptr))
            {
                registry = reinterpret_cast<void*>(GetGlobalInt());
                void* object = LookupObjectById(registry, nullptr);
                if (((bool (__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr))
                {
                    return 0;
                }
            }
        }
        return g_DefWindowProcA(window, 0x85u, wParam, lParam);

    case 258:
        if (g_pMenuManager)
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            void* inputHandler = CSF_AT(void*, state, 0x10);
            ((void (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, inputHandler, 0)[0x68 / 4])(
                    inputHandler, nullptr, wParam);
        }
        return 0;

    default:
        return g_DefWindowProcA(window, message, wParam, lParam);
    }
}

HOOK(0x00508D40, AppMain_WindowProcedure, "auto");
