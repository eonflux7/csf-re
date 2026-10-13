#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, DeleteScheduleGroupBase, 0x006FDB90, (int objectAddress));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));

static int __fastcall SceneLoader_ResetRenderMember(C_0085e7b8* self, void* /*edx*/)
{
    // Release every object in the group array (count cached once, array re-read each pass).
    uint32_t groupCount = self->field_000c;
    uint32_t index = 0;
    if (groupCount != 0)
    {
        do
        {
            uint32_t* groupArray = reinterpret_cast<uint32_t*>(self->field_0004);
            DeleteScheduleGroupBase(static_cast<int>(groupArray[index++]));
        } while (index < groupCount);
    }

    // Destroy the render texture held at +0x90, then clear its companion slot at +0x94.
    uint32_t renderTexture = CSF_AT(uint32_t, self, 0x90);
    if (renderTexture != 0)
    {
        RwTextureDestroy(static_cast<int>(renderTexture));
        CSF_AT(uint32_t, self, 0x94) = 0;
    }

    CSF_AT(uint32_t, self, 0x90) = 0;
    CSF_AT(uint32_t, self, 0x94) = 0;
    self->field_000c = 0;
    return 0;
}

HOOK(0x006D5BB0, SceneLoader_ResetRenderMember, "auto");
