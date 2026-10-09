#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833908_Activate, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833908_ApplyByteParam, 0x0049A7F0, (void* self, void* edx, uint32_t byteArg));

// Resolves the handle record referenced through *refPointers, then either activates the
// resolved object or applies the byte parameter from refPointers[1] to it.
static int __stdcall SceneDispatch_ResolveHandleFromRef(int /*unusedA1*/, int /*unusedA2*/, uint32_t* refPointers)
{
    uint8_t* refRecord = reinterpret_cast<uint8_t*>(refPointers[0]);
    uint32_t handleRecordAddr = *CSF_AT(uint32_t*, refRecord, 4);
    uint8_t* handleRecord = reinterpret_cast<uint8_t*>(handleRecordAddr);
    int handleIndex = *CSF_AT(int*, handleRecord, 12);

    int result = static_cast<int>(handleRecordAddr + 4);
    if (handleIndex >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        C_00833908* target = reinterpret_cast<C_00833908*>(result);
        if (result != 0)
        {
            if (*CSF_AT(int*, target, 0x50) != 0 ||
                GetSceneMenuContext(g_pCurrentScene, nullptr) == static_cast<uint32_t>(result))
            {
                return C_00833908_Activate(target, nullptr);
            }
            else
            {
                uint8_t* argRecord = reinterpret_cast<uint8_t*>(refPointers[1]);
                uint8_t* argSource = reinterpret_cast<uint8_t*>(*CSF_AT(uint32_t*, argRecord, 4));
                uint32_t argByte = *CSF_AT(uint8_t*, argSource, 1);
                return C_00833908_ApplyByteParam(target, argSource, argByte);
            }
        }
    }
    return result;
}

HOOK(0x004614E0, SceneDispatch_ResolveHandleFromRef, "auto");
