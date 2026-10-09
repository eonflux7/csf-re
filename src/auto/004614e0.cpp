#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833908_ApplyByteParam, 0x0049A7F0, (void* self, void* edx, uint32_t byteArg));

// Resolves the handle stored in a reference record through the current scene's handle table.
// A valid, non-menu target is either made unavailable or receives the byte parameter of the
// second argument record; otherwise the resolved value (or the inner handle slot) is returned.
static int __stdcall SceneDispatch_ResolveHandleFromRef(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    uint32_t recordPtr = a3[0];
    uint32_t handleRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(recordPtr), 4);
    int handleFlags = CSF_AT(int, reinterpret_cast<void*>(handleRecord), 12);
    uint32_t handleAddress = handleRecord + 4;

    if (handleFlags < 0)
        return static_cast<int>(handleAddress);

    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddress));
    if (resolved == 0)
        return resolved;

    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(resolved)));

    if (CSF_AT(uint32_t, target, 0x50) != 0
        || GetSceneMenuContext(g_pCurrentScene, nullptr) == static_cast<uint32_t>(resolved))
    {
        return HandleTargetUnavailable(target, nullptr);
    }

    uint32_t byteRecord = a3[1];
    uint32_t byteInner = CSF_AT(uint32_t, reinterpret_cast<void*>(byteRecord), 4);
    uint8_t byteParam = CSF_AT(uint8_t, reinterpret_cast<void*>(byteInner), 1);
    return C_00833908_ApplyByteParam(target, nullptr, static_cast<uint32_t>(byteParam));
}

HOOK(0x4614E0, SceneDispatch_ResolveHandleFromRef, "auto");
