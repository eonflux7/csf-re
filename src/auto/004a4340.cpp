#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Stage_FlushBeforeReset, 0x004A2C00, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));

static int __fastcall C_00834598_FlushAndClearSceneHandle(C_00834598* self, void* /*edx*/)
{
    Stage_FlushBeforeReset(self, nullptr);

    int result = self->field_008c;
    if (result)
    {
        int globalInt = GetGlobalInt();
        void* sceneController = reinterpret_cast<void*>(SceneLoader_GetSceneController(reinterpret_cast<void*>(globalInt), nullptr));
        result = reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t)>(CSF_AT(void**, sceneController, 0)[6])(
            sceneController, nullptr, self->field_008c);
    }

    self->field_008c = 0;
    return result;
}

HOOK(0x4A4340, C_00834598_FlushAndClearSceneHandle, "auto");
