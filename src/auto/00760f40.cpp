#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, AppConfig_InitializeRegionObject, 0x00760EC0, (void* self, int a2, void* a3, char* a4));
CSF_GLOBAL(int, g_Value_8BC1E4, 0x008BC1E4);

static char __cdecl AppConfig_InitializeRegionSingleton(const char* configName)
{
    char initOk = 0;

    if (g_Value_8BC1E4 == 0)
    {
        void* regionObject = Mem_Alloc(0x130);
        if (regionObject != nullptr)
        {
            void* initResult = AppConfig_InitializeRegionObject(
                regionObject,
                static_cast<int>(reinterpret_cast<std::uintptr_t>(configName)),
                const_cast<char*>(configName),
                &initOk);
            g_Value_8BC1E4 = static_cast<int>(reinterpret_cast<std::uintptr_t>(initResult));
            return initOk;
        }
        g_Value_8BC1E4 = 0;
    }
    return 0;
}

HOOK(0x00760F40, AppConfig_InitializeRegionSingleton, "auto");
