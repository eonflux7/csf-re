#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

using AdjustWindowRectFn = int (__stdcall*)(void*, std::uint32_t, int);
using GetSystemMetricsFn = int (__stdcall*)(int);
using MoveWindowFn = int (__stdcall*)(void*, int, int, int, int, int);

CSF_GLOBAL(AdjustWindowRectFn, g_AdjustWindowRect, 0x0082D2C0);
CSF_GLOBAL(GetSystemMetricsFn, g_GetSystemMetrics, 0x0082D2BC);
CSF_GLOBAL(MoveWindowFn, g_MoveWindow, 0x0082D2B8);

static int __cdecl AppMain_ResizeAndCenterWindow(
    void* window, std::int32_t width, std::int32_t height)
{
    alignas(8) std::uint8_t rectBytes[0x10];
    CSF_AT(std::int32_t, rectBytes, 0x00) = 0;
    CSF_AT(std::int32_t, rectBytes, 0x04) = 0;

    if (width <= 0)
    {
        void* object = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        width = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x20 / 4])(object, nullptr);
    }
    CSF_AT(std::int32_t, rectBytes, 0x08) = width;

    if (height <= 0)
    {
        void* object = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        height = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x24 / 4])(object, nullptr);
    }
    CSF_AT(std::int32_t, rectBytes, 0x0C) = height;

    g_AdjustWindowRect(rectBytes, 0xCA0000u, 0);

    const int outerWidth = CSF_AT(std::int32_t, rectBytes, 0x08)
                         - CSF_AT(std::int32_t, rectBytes, 0x00);
    const int outerHeight = CSF_AT(std::int32_t, rectBytes, 0x0C)
                          - CSF_AT(std::int32_t, rectBytes, 0x04);
    const int screenWidth = g_GetSystemMetrics(0);

    return g_MoveWindow(
        window, (screenWidth - outerWidth) / 2, 0,
        outerWidth, outerHeight, 1);
}

HOOK(0x00508F50, AppMain_ResizeAndCenterWindow, "auto");
