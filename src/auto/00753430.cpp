#include <cstdint>
#include "globals.h"
#include "hook.h"

static CSF_FUNC(uint32_t*, __fastcall, DestroySpawnMessage, 0x00753440, (void* self, void* edx, char a2));

static uint32_t* __fastcall NetSpawnMessages_DestroyFromSecondaryBase(
    void* self, void* /*edx*/, char flags)
{
    return DestroySpawnMessage(
        static_cast<uint8_t*>(self) - 0x154, nullptr, flags);
}

HOOK(0x00753430, NetSpawnMessages_DestroyFromSecondaryBase, "auto");
