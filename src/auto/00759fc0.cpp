#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, ConfigObject_Initialize, 0x00759F20, (void* self, int a2, void* a3, char* a4));
CSF_GLOBAL(uint32_t, g_Value_8BC1E0, 0x008BC1E0);

static char __cdecl GameConfig_InitializeSecondarySingleton(char* configName)
{
    char initOk = 0;

    if (g_Value_8BC1E0 == 0)
    {
        void* configObject = Mem_Alloc(8888);
        if (configObject != nullptr)
        {
            void* initResult = ConfigObject_Initialize(
                configObject,
                static_cast<int>(reinterpret_cast<std::uintptr_t>(configName)),
                configName,
                &initOk);
            g_Value_8BC1E0 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(initResult));
            return initOk;
        }
        g_Value_8BC1E0 = 0;
    }
    return 0;
}

HOOK(0x00759FC0, GameConfig_InitializeSecondarySingleton, "auto");
