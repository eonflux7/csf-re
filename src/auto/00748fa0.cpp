#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateAuthoritativeState, 0x0064C980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateReplicaState, 0x0064CA10, (void* self, void* edx));

static char __fastcall NetMpStats_UpdateCogibleState(
    void* self, void* /*edx*/, int /*unused*/)
{
    void* owner = static_cast<std::uint8_t*>(self) - 0x440;
    CSF_AT(std::uint32_t, self, 0x0C) = 1;

    if (static_cast<unsigned char>(Sub_64B250(owner, nullptr)) != 0)
        return NetCogibleDDL_UpdateAuthoritativeState(owner, nullptr);

    return NetCogibleDDL_UpdateReplicaState(owner, nullptr);
}

HOOK(0x00748FA0, NetMpStats_UpdateCogibleState, "auto");
