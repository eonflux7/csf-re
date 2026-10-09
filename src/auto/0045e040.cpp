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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char, __fastcall, sub_452BA0, 0x00452BA0, (void* self, void* edx, unsigned int a2, char a3));

// __stdcall(a1, a2, handle): handle -> object -> sub-object (+0x4) -> control index (+0x20).
// When the index is valid, applies it to the current scene's control table.
static char __stdcall ApplySceneControlFromHandle(int /*a1*/, int /*a2*/, int handle)
{
    uint32_t objectAddress = CSF_AT(uint32_t, reinterpret_cast<void*>(handle), 0);
    uint32_t subObjectAddress = CSF_AT(uint32_t, reinterpret_cast<void*>(objectAddress), 0x4);
    uint32_t controlIndex = CSF_AT(uint32_t, reinterpret_cast<void*>(subObjectAddress), 0x20);

    char result = static_cast<char>(controlIndex);
    if (controlIndex != 0xFFFFFFFFu)
        result = sub_452BA0(g_pCurrentScene, nullptr, controlIndex, 1);
    return result;
}

HOOK(0x0045E040, ApplySceneControlFromHandle, "auto");
