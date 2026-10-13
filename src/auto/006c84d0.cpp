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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));

// Releases the three handles owned by C_00860908 (clump manager, object registry,
// scene controller), zeroing each one after its release call, then restores the base vtable.
static int __fastcall C_00860908_ReleaseHandles(C_00860908* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860908);

    if (self->field_000c)
    {
        void* clumpManager = reinterpret_cast<void*>(static_cast<uintptr_t>(ResolveClumpManager(static_cast<int>(GetGlobalInt()), nullptr)));
        using ReleaseFn = void(__fastcall*)(void* self, void* edx, uint32_t handle);
        reinterpret_cast<ReleaseFn>(CSF_AT(void**, clumpManager, 0)[11])(clumpManager, nullptr, self->field_000c);
        self->field_000c = 0;
    }

    if (self->field_0004)
    {
        void* objectRegistry = reinterpret_cast<void*>(static_cast<uintptr_t>(SceneLoader_GetObjectRegistry(reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt())), nullptr)));
        using ReleaseFn = void(__fastcall*)(void* self, void* edx, uint32_t handle);
        reinterpret_cast<ReleaseFn>(CSF_AT(void**, objectRegistry, 0)[11])(objectRegistry, nullptr, self->field_0004);
        self->field_0004 = 0;
    }

    int result = static_cast<int>(self->field_0008);
    if (result)
    {
        void* sceneController = reinterpret_cast<void*>(static_cast<uintptr_t>(SceneLoader_GetSceneController(reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt())), nullptr)));
        result = ((int(__fastcall*)(void*, void*, C_00833908*))CSF_AT(void**, sceneController, 0)[6])(
            sceneController, nullptr, reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(self->field_0008)));
        self->field_0008 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x6C84D0, C_00860908_ReleaseHandles, "auto");
