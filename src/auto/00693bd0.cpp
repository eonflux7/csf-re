#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, CorePaths_DestroyOwnedComponent, 0x00693B80, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

CSF_GLOBAL(uint32_t, g_ComponentPools, 0x008BAF48);

static char __fastcall S_00693740_ReleaseResources(
    S_00693740* self, void* /*edx*/)
{
    if (self->field_0014)
    {
        if (self->field_0000)
        {
            CorePaths_DestroyOwnedComponent(
                reinterpret_cast<void*>(self->field_0000), nullptr, 1);
            self->field_0000 = 0;
        }

        for (uint32_t i = 0; i < self->field_0024; ++i)
        {
            uint32_t block =
                reinterpret_cast<uint32_t*>(self->field_001c)[i];
            if (block)
            {
                CoreMemory_Free(reinterpret_cast<void*>(block));
                reinterpret_cast<uint32_t*>(self->field_001c)[i] = 0;
            }
        }

        for (uint32_t i = 0; i < self->field_0034; ++i)
        {
            uint32_t block =
                reinterpret_cast<uint32_t*>(self->field_002c)[i];
            if (block)
            {
                CoreMemory_Free(reinterpret_cast<void*>(block));
                reinterpret_cast<uint32_t*>(self->field_002c)[i] = 0;
            }
        }
    }
    else
    {
        (&g_ComponentPools)[self->field_0015] = 0;
    }

    uint32_t result = self->field_0004;
    self->field_0024 = 0;
    self->field_0034 = 0;

    if (result)
    {
        result = CoreMemory_Free(reinterpret_cast<void*>(result));
        self->field_0004 = 0;
    }

    void* embeddedOwner = reinterpret_cast<void*>(self->field_000c);
    if (embeddedOwner)
    {
        ResetEmbeddedMemberAt8(
            reinterpret_cast<void**>(embeddedOwner), nullptr);
        result = CoreMemory_Free(embeddedOwner);
        self->field_000c = 0;
    }

    return static_cast<char>(result);
}

HOOK(0x00693BD0, S_00693740_ReleaseResources, "auto");
