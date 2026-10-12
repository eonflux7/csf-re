#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
static CSF_FUNC(uint32_t*, __fastcall, C_00868af0_InitializeSpawnBuffer, 0x007521D0, (C_00868af0* self, void* , uint32_t* dimensions, int count));

static uint32_t* __cdecl NetSpawnMessages_CreateSpawnBuffer(uint32_t* dimensions)
{
    auto* buffer = static_cast<C_00868af0*>(NetZ_OperatorNew(40));
    if (buffer)
        return C_00868af0_InitializeSpawnBuffer(buffer, nullptr, dimensions, 4);
    return nullptr;
}

HOOK(0x007522C0, NetSpawnMessages_CreateSpawnBuffer, "auto");
