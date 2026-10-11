#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetLoadedScene, 0x006CBAC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(int, __cdecl, RpWorldAddClump, 0x0058E280, (int a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RpWorldRemoveClump, 0x0058E310, (int a1, int a2));

// Walks the clump list of the scene-loader object and makes sure each clump
// lives in the currently loaded world: removes it from its old world (if any)
// and adds it to the loaded one. Returns the number of entries visited.
static unsigned int __fastcall C_00860bb8_SyncClumpsToLoadedWorld(C_00860bb8* self, void* /*edx*/)
{
    unsigned int result = 0;
    for (uint32_t i = 0; i < self->field_001c; ++i)
    {
        C_0085e7b8* clumpEntry = *reinterpret_cast<C_0085e7b8**>(self->field_0014 + 4 * result);
        if (clumpEntry)
        {
            int cachedObjectProvider = reinterpret_cast<int>(GetCachedObjectProvider(clumpEntry, nullptr));
            if (cachedObjectProvider)
            {
                int loadedScene = SceneLoader_GetLoadedScene(self, nullptr);
                int worldFromClump = S_006fdfe0_FindWorldFromClump(cachedObjectProvider);
                if (worldFromClump != loadedScene)
                {
                    if (loadedScene)
                    {
                        if (worldFromClump)
                            RpWorldRemoveClump(worldFromClump, cachedObjectProvider);
                        RpWorldAddClump(loadedScene, reinterpret_cast<uint32_t*>(cachedObjectProvider));
                    }
                }
            }
        }
        result = i + 1;
    }
    return result;
}

HOOK(0x006CBB40, C_00860bb8_SyncClumpsToLoadedWorld, "auto");
