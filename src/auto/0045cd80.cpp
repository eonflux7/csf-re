#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* AsAddressPtr(uint32_t address) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

// Looks up the handle stored in the task's +4 block, resolves it in the current scene,
// and writes a 0/1 ready flag to the byte at +1 of the object referenced by the first argument.
static int __stdcall SceneHandle_SetReadyFlagFromTask(int a1, int a2, int a3) {
    uint32_t taskObject = CSF_AT(uint32_t, AsAddressPtr(static_cast<uint32_t>(a3)), 0);
    uint32_t taskInner = CSF_AT(uint32_t, AsAddressPtr(taskObject), 4);
    int32_t handleField = CSF_AT(int32_t, AsAddressPtr(taskInner), 12);
    uint32_t handleSlotAddress = taskInner + 4;

    if (handleField < 0) {
        uint32_t flagOwner = CSF_AT(uint32_t, AsAddressPtr(static_cast<uint32_t>(a1)), 4);
        CSF_AT(uint8_t, AsAddressPtr(flagOwner), 1) = 0;
        return static_cast<int>(flagOwner);
    }

    void* resolved = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleSlotAddress)));
    uint32_t resolvedAddress = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(resolved));

    if (resolvedAddress == 0 || CSF_AT(int32_t, AsAddressPtr(resolvedAddress), 80) != 0) {
        uint32_t flagOwner = CSF_AT(uint32_t, AsAddressPtr(static_cast<uint32_t>(a1)), 4);
        CSF_AT(uint8_t, AsAddressPtr(flagOwner), 1) = 0;
        return 0;
    }

    uint32_t flagOwner = CSF_AT(uint32_t, AsAddressPtr(static_cast<uint32_t>(a1)), 4);
    CSF_AT(uint8_t, AsAddressPtr(flagOwner), 1) = 1;
    return 1;
}

HOOK(0x0045CD80, SceneHandle_SetReadyFlagFromTask, "auto");
