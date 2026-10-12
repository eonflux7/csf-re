#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
static CSF_FUNC(char*, __fastcall, C_00868b04_InitializeSpawnBuffer, 0x007523E0, (void* self, void* edx, int a2, int a3));

static char* __cdecl NetSpawnMessages_CreateThreeElementSpawnBuffer(int parameters)
{
    void* buffer = NetZ_OperatorNew(52);
    if (buffer)
        return C_00868b04_InitializeSpawnBuffer(buffer, nullptr, parameters, 3);
    return nullptr;
}

HOOK(0x007524D0, NetSpawnMessages_CreateThreeElementSpawnBuffer, "auto");
