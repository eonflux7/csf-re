#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees bound by address (signatures from the packet).
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, SceneEntity_Activate, 0x004E51B0, (void* self, void* edx, char a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads a handle from the object referenced by *a3 and, if the handle is valid,
// resolves it in the current scene and calls SceneEntity_Activate with flag 0
// when the entity pointer stored at +60 is non-null.
static int __stdcall DeactivateSceneEntityFromRef(int a1, int a2, int a3)
{
    // a3 points to a slot holding a pointer to a wrapper object.
    uint32_t wrapper = CSF_AT(uint32_t, a3, 0);
    uint32_t handleOwner = CSF_AT(uint32_t, wrapper, 4);
    int handleField = CSF_AT(int, handleOwner, 12);
    int result = static_cast<int>(handleOwner + 4);

    if (handleField >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result) {
            result = CSF_AT(int, result, 60);
            if (result) {
                return static_cast<int>(SceneEntity_Activate(
                    reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(result))),
                    nullptr,
                    static_cast<char>(0)));
            }
        }
    }
    return result;
}

HOOK(0x0045ED30, DeactivateSceneEntityFromRef, "auto");
