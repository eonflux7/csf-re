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

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(std::uint32_t*, __fastcall, ParticleManager_InitializeEmptyWord, 0x0051F690, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, CoreZlibLog_ConnectLogWindow, 0x006A1EC0, (void** self, void*, char* format, char));

CSF_GLOBAL(int, g_SceneLoaderBlock, 0x008BAFD8);
CSF_GLOBAL(std::uintptr_t, g_IsWindowImportSlot, 0x0082D288);
CSF_GLOBAL(std::uintptr_t, g_GetWindowTextAImportSlot, 0x0082D2F8);
CSF_GLOBAL(std::uintptr_t, g_SetForegroundWindowImportSlot, 0x0082D28C);

static void __cdecl AppMain_ConfigureWindow(void* hWnd, char a2)
{
    if (a2)
    {
        int allocatedBlock = CoreMemory_AllocUntagged(4);
        if (allocatedBlock)
        {
            g_SceneLoaderBlock = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                ParticleManager_InitializeEmptyWord(reinterpret_cast<std::uint32_t*>(allocatedBlock), nullptr)));
        }
        else
        {
            g_SceneLoaderBlock = 0;
        }

        if (reinterpret_cast<int(__stdcall*)(void*)>(g_IsWindowImportSlot)(hWnd))
        {
            char windowTitle[128];
            reinterpret_cast<int(__stdcall*)(void*, char*, int)>(g_GetWindowTextAImportSlot)(hWnd, windowTitle, 128);
            CoreZlibLog_ConnectLogWindow(reinterpret_cast<void**>(g_SceneLoaderBlock), nullptr, windowTitle, a2);
        }
        else
        {
            CoreZlibLog_ConnectLogWindow(reinterpret_cast<void**>(g_SceneLoaderBlock), nullptr, nullptr, a2);
        }

        reinterpret_cast<int(__stdcall*)(void*)>(g_SetForegroundWindowImportSlot)(hWnd);
    }
}

HOOK(0x0069CEE0, AppMain_ConfigureWindow, "auto");
