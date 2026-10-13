#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
static CSF_FUNC(uint8_t*, __fastcall, InitializeSceneLoadTimer, 0x0069CD10, (void* self, void* edx, int a2));

static uint8_t* __cdecl SceneLoader_CreateLoadTimer()
{
    void* timerBlock = reinterpret_cast<void*>(CoreMemory_AllocUntagged(64));
    if (timerBlock)
        return InitializeSceneLoadTimer(timerBlock, nullptr, 1);
    return nullptr;
}

HOOK(0x0069CDC0, SceneLoader_CreateLoadTimer, "auto");
