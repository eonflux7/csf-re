#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, HandleEventType4, 0x004012C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, HandleEventType14Or15, 0x00401550, (int a1));

static void __fastcall C_0082d46c_DispatchMessage(C_0082d46c* self, void* /*edx*/, int a2)
{
    uint8_t* globalState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    if (CSF_AT(uint8_t, globalState, 0xD0) != 0)
        return;

    uint8_t* message = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
    int messageType = CSF_AT(int, message, 4);

    if (messageType == 4)
    {
        HandleEventType4(self, nullptr, a2);
    }
    else if (messageType == 14 || messageType == 15)
    {
        HandleEventType14Or15(a2);
    }
}

HOOK(0x401A70, C_0082d46c_DispatchMessage, "auto");
