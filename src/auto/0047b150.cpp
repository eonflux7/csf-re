#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D900, 0x0050D900, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// __thiscall(char a2) with ret 4: self in ecx, one stack argument.
static char __fastcall AttackTasks_SetFieldTransitionScale(uint32_t* self, void* /*edx*/, char enableScale)
{
    if (enableScale)
    {
        float transitionScale = 1.0f;
        uint8_t* owner = reinterpret_cast<uint8_t*>(self[5]);
        if (CSF_AT(int, owner, 0x50) == 1)
        {
            int globalState = GetGlobalState_50D190();
            transitionScale = MenuSystem_GetTransitionScale_50D900(reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr);
        }

        int globalValue = GetGlobalValue_8BC1E0();
        uint8_t* globalObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(globalValue));
        uint8_t* target = reinterpret_cast<uint8_t*>(self[2]);
        CSF_AT(float, target, 0x2B4) = transitionScale * CSF_AT(float, globalObj, 0xAA8);
        return static_cast<char>(globalValue);
    }

    uint8_t* target = reinterpret_cast<uint8_t*>(self[2]);
    CSF_AT(int, target, 0x2B4) = 0;
    return enableScale;
}

HOOK(0x0047B150, AttackTasks_SetFieldTransitionScale, "auto");
