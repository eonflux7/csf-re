#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateAuthoritativeState, 0x0064C980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateReplicaState, 0x0064CA10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_749b20_TakeByteFlag, 0x00749B20, (void* self, void*));
CSF_FUNC(char, __fastcall, SetFlag48, 0x00753490, (uint8_t* self, void*));

static char __fastcall NetSpawnMessages_UpdateCogibleState(
    void* self, void* /*edx*/, int /*unused*/)
{
    auto* bytes = static_cast<uint8_t*>(self);
    auto* flagObject = bytes - 0x110;

    CSF_AT(uint32_t, self, 0x0C) = 4;
    if (C_749b20_TakeByteFlag(flagObject, nullptr))
        SetFlag48(flagObject, nullptr);

    void* cogible = bytes - 0x154;
    if (static_cast<uint8_t>(Sub_64B250(cogible, nullptr)) != 0)
        return NetCogibleDDL_UpdateAuthoritativeState(cogible, nullptr);

    return NetCogibleDDL_UpdateReplicaState(cogible, nullptr);
}

HOOK(0x00753390, NetSpawnMessages_UpdateCogibleState, "auto");
