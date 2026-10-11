#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, GetSpawnFlagFromGlobal, 0x0073CD30, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ApplySpawnFlag, 0x007410F0, (void* self, void* edx, int a2));

// Resolves the handle at a1+8 in the current scene, reads the object's +0x18 link,
// and if it is not the sentinel 1712 (0x6B0), applies the global spawn flag to the
// sub-object located at link - 0x6B0.
static int __stdcall MpSpawn_ResolveHandleAndApplySpawnFlag(int a1)
{
    int result = CHandleTable_Resolve(g_pCurrentScene, nullptr, a1 + 8);
    if (result) {
        result = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(result)), 0x18);
        if (result) {
            if (result != 1712) {
                C_00858180* spawn = reinterpret_cast<C_00858180*>(
                    static_cast<uintptr_t>(result - 1712));
                int globalValue = GetGlobalDword_73C020();
                char flag = static_cast<char>(
                    GetSpawnFlagFromGlobal(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr));
                return ApplySpawnFlag(spawn, nullptr, static_cast<int>(flag));
            }
        }
    }
    return result;
}

HOOK(0x00551260, MpSpawn_ResolveHandleAndApplySpawnFlag, "auto");
