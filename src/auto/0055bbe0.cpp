#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, MpRules_InitializeSabotageObject, 0x0055BB60, (void* self, void* edx, char* a2, uint8_t* a3));
CSF_GLOBAL(int, g_Value_8B08F8, 0x008B08F8);

static char __cdecl MpRules_InitializeSabotageSingleton(char* a2)
{
    char initOk = 0;

    if (g_Value_8B08F8 == 0)
    {
        void* sabotageObject = Mem_Alloc(0x15C);
        if (sabotageObject != nullptr)
        {
            uint32_t* initResult = MpRules_InitializeSabotageObject(sabotageObject, nullptr, a2, reinterpret_cast<uint8_t*>(&initOk));
            g_Value_8B08F8 = static_cast<int>(reinterpret_cast<std::uintptr_t>(initResult));
            return initOk;
        }
        g_Value_8B08F8 = 0;
    }
    return 0;
}

HOOK(0x0055BBE0, MpRules_InitializeSabotageSingleton, "auto");
