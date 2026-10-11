#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Initializer for the freshly allocated scene loader object (__fastcall: ecx=this, edx unused).
CSF_FUNC(int, __fastcall, SceneLoader_Initialize, 0x00713160, (void* self, void* edx));

static int __cdecl GameBddLoader_AllocAndInitSceneLoader()
{
    void* loaderObject = Mem_Alloc(3268);
    if (loaderObject)
        return SceneLoader_Initialize(loaderObject, nullptr);
    return 0;
}

HOOK(0x00506CD0, GameBddLoader_AllocAndInitSceneLoader, "auto");
