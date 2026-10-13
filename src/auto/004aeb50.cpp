#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

CSF_GLOBAL(uint32_t*, g_CoverCandidateData, 0x008997E0);
CSF_GLOBAL(uint32_t, g_CoverCandidateCapacity, 0x008997E4);
CSF_GLOBAL(uint32_t, g_CoverCandidateCount, 0x008997E8);

static uint32_t __cdecl AiFncTasks_ResetCoverCandidateData()
{
    uint32_t result = reinterpret_cast<uint32_t>(g_CoverCandidateData);

    g_CoverCandidateCapacity = 0;
    g_CoverCandidateCount = 0;

    if (result)
    {
        result = static_cast<uint32_t>(CoreMemory_Free(reinterpret_cast<void*>(result)));
        g_CoverCandidateData = nullptr;
    }

    return result;
}

HOOK(0x004AEB50, AiFncTasks_ResetCoverCandidateData, "auto");
