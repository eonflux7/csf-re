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

// Callees, bound by address.
CSF_FUNC(unsigned int, __fastcall, MpCharacterSelect_ReleasePreviewScene, 0x005554A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, MpCharacterSelect_CreatePreviewScene, 0x00555550, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Global holding the current character-select preview scene object.
CSF_GLOBAL(C_00844694*, g_Int8B0688, 0x008B0688);

static bool __cdecl MpCharacterSelect_RefreshPreviewCamera(int a1, int a2)
{
    // Tear down the existing preview scene, if any.
    if (g_Int8B0688)
    {
        C_00844694* oldScene = g_Int8B0688;
        MpCharacterSelect_ReleasePreviewScene(oldScene, nullptr);
        Mem_Free(oldScene);
        g_Int8B0688 = nullptr;
    }

    // Allocate and build the new preview scene (size 0x14B0 = 5296).
    void* block = Mem_Alloc(5296);
    if (block)
    {
        g_Int8B0688 = reinterpret_cast<C_00844694*>(
            MpCharacterSelect_CreatePreviewScene(block, nullptr, a1));
        return g_Int8B0688 != nullptr;
    }

    g_Int8B0688 = nullptr;
    return false;
}

HOOK(0x555DD0, MpCharacterSelect_RefreshPreviewCamera, "auto");
