#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (void* buffer, void* edx));
CSF_FUNC(unsigned int, __fastcall, MenuBuffer_Destroy, 0x004FF3C0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(unsigned int, __fastcall, Game_DispatchMenuRequest, 0x0050F790, (void* self, void* edx, int a2, int a3, int a4, void* a5));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __stdcall, sub_521F40, 0x00521F40, (int a1, int a2));

static unsigned int __cdecl SceneLoader_RunLoadMenuRequest()
{
    alignas(4) uint8_t menuBuffer[0xB4];

    MenuBuffer_Init(menuBuffer, nullptr);

    CSF_AT(uint8_t, menuBuffer, 0x0C) = 0;   // ebp-0xA8
    CSF_AT(uint8_t, menuBuffer, 0x19) = 1;   // ebp-0x9B
    CSF_AT(uint8_t, menuBuffer, 0xA8) = 0;   // ebp-0x0C

    GetGlobalValue_521BB0();
    sub_521F40(1, 0);

    int globalState = GetGlobalState_50D190();
    Game_DispatchMenuRequest(reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)),
                             nullptr, 0x73743031, 1, 0, menuBuffer);

    Nullsub_506AC0(0);

    return MenuBuffer_Destroy(menuBuffer, nullptr);
}

HOOK(0x5018D0, SceneLoader_RunLoadMenuRequest, "auto");
