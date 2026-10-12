#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, MpRules_InitializeSabotageObject, 0x0055A910, (void* self, void* edx, char* a2, char* a3));
CSF_GLOBAL(int, g_Value_8B08F4, 0x008B08F4);

static char __cdecl MpRules_InitializeSabotageSingleton(char* configName)
{
    char initOk = 0;

    if (g_Value_8B08F4 == 0)
    {
        void* sabotageObject = Mem_Alloc(0x208);
        if (sabotageObject != nullptr)
        {
            int initResult = MpRules_InitializeSabotageObject(sabotageObject, nullptr, configName, &initOk);
            g_Value_8B08F4 = initResult;
            return initOk;
        }
        g_Value_8B08F4 = 0;
    }
    return 0;
}

HOOK(0x0055A970, MpRules_InitializeSabotageSingleton, "auto");
