#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, GameConfig_InitializeObject, 0x00755740, (void* self, void* edx, char* a2, char* a3));
CSF_GLOBAL(int, g_GlobalValue, 0x008BC1DC);

static char __cdecl GameConfig_InitializeSingleton(char* configName)
{
    char initOk = 0;

    if (g_GlobalValue == 0)
    {
        void* configObject = Mem_Alloc(0x250);
        if (configObject != nullptr)
        {
            uint32_t* initResult = GameConfig_InitializeObject(configObject, nullptr, configName, &initOk);
            g_GlobalValue = static_cast<int>(reinterpret_cast<std::uintptr_t>(initResult));
            return initOk;
        }
        g_GlobalValue = 0;
    }
    return 0;
}

HOOK(0x007557C0, GameConfig_InitializeSingleton, "auto");
