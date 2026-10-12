#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(uint32_t*, __fastcall, NetSpawnMessages_DestroyCompleteObject, 0x00753330, (void* self, void* edx, char a2));

static uint32_t* __fastcall NetSpawnMessages_DestroySecondaryBase(
    void* self, void* /*edx*/, char flags)
{
    return NetSpawnMessages_DestroyCompleteObject(
        static_cast<uint8_t*>(self) - 216, nullptr, flags);
}

HOOK(0x00753320, NetSpawnMessages_DestroySecondaryBase, "auto");
