#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __cdecl, Campaign_CheckScenarioFile, 0x00697720, (char* a1));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, RtFSFileSystemState_Construct, 0x0069F980, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

// Opens a scenario file from the Maps directory and, when it is valid, stores a
// newly constructed file-system state object in g_pOptions->field_0060.
static char __cdecl CoreFiles_OpenScenarioAndStoreFileSystemState(char* scenarioPath)
{
    // Validate the scenario file first; bail out with 0 if it fails.
    if (!Campaign_CheckScenarioFile(scenarioPath))
        return 0;

    void* openedFile = CoreFiles_OpenSceneFile(scenarioPath, (char*)CSF_STR(0x0083E130, "r"), 0);
    if (!openedFile)
        return 0;

    int stateBlock = CoreMemory_AllocUntagged(68);
    if (stateBlock)
    {
        uint32_t* fileSystemState = RtFSFileSystemState_Construct(
            reinterpret_cast<void*>(stateBlock), nullptr, reinterpret_cast<int>(openedFile));
        CSF_AT(uint32_t, g_pOptions, 0x60) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(fileSystemState));
        return static_cast<char>(reinterpret_cast<uintptr_t>(fileSystemState));
    }

    CSF_AT(uint32_t, g_pOptions, 0x60) = 0;
    return 0;
}

HOOK(0x698680, CoreFiles_OpenScenarioAndStoreFileSystemState, "auto");
