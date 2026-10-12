#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
static CSF_FUNC(uint32_t*, __fastcall, InitializeSpawnMessage, 0x007AB350, (void* self, void* edx, int a2));

static uint32_t* __cdecl NetSpawnMessages_CreateType50Message()
{
    void* message = NetZ_OperatorNew(28);
    if (message)
        return InitializeSpawnMessage(message, nullptr, 50);
    return nullptr;
}

HOOK(0x00751DE0, NetSpawnMessages_CreateType50Message, "auto");
