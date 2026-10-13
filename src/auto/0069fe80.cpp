#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Original: 0x00697980 CloseSceneFile(void* file), __cdecl
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));

// Closes the scene file handle stored at +0x40, clears that slot,
// and marks the state flag at +0x20 as 1. Returns CloseSceneFile's result.
static int __cdecl RtFs_CloseSceneFileAndReset(int a1)
{
    auto* base = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a1));

    std::uint32_t sceneFileHandle = CSF_AT(std::uint32_t, base, 0x40);
    int result = static_cast<int>(
        CloseSceneFile(reinterpret_cast<void*>(static_cast<std::uintptr_t>(sceneFileHandle))));

    CSF_AT(std::uint32_t, base, 0x40) = 0;
    CSF_AT(std::uint32_t, base, 0x20) = 1;
    return result;
}

HOOK(0x0069FE80, RtFs_CloseSceneFileAndReset, "auto");
