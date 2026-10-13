#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RpWorldCreate, 0x0058CCB0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RwFrameSetStaticPlugins, 0x006329C0, (int a1));
CSF_FUNC(void*, __fastcall, SceneLoader_FinalizeRenderResources, 0x006BDCB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_008605d8_ProcessContextObject, 0x006BE9C0, (C_008605d8* self, void*));
CSF_FUNC(char, __fastcall, C_008605d8_LoadEnvironmentStream, 0x006BEC60, (C_008605d8* self, void* , unsigned int source, char buffered));

static char __fastcall C_008605d8_LoadEnvironment(
    C_008605d8* self, void* /*edx*/, const void* sourceDescriptor, char buffered)
{
    SceneLoader_FinalizeRenderResources(self, nullptr);

    self->field_0008 = CSF_AT(uint8_t, sourceDescriptor, 0x00);
    RwFrameSetStaticPlugins(buffered ? 0 : 1);

    const char result = C_008605d8_LoadEnvironmentStream(
        self, nullptr, CSF_AT(unsigned int, sourceDescriptor, 0x04), buffered);

    if (!buffered)
        C_008605d8_ProcessContextObject(self, nullptr);

    if (!self->field_0004)
    {
        alignas(8) uint8_t worldBoundsBytes[0x18];
        CSF_AT(float, worldBoundsBytes, 0x00) = 1.0f;
        CSF_AT(float, worldBoundsBytes, 0x04) = 1.0f;
        CSF_AT(float, worldBoundsBytes, 0x08) = 1.0f;
        CSF_AT(float, worldBoundsBytes, 0x0C) = -1.0f;
        CSF_AT(float, worldBoundsBytes, 0x10) = -1.0f;
        CSF_AT(float, worldBoundsBytes, 0x14) = -1.0f;

        self->field_0004 = reinterpret_cast<uint32_t>(
            RpWorldCreate(reinterpret_cast<uint32_t*>(worldBoundsBytes)));
    }

    const uint32_t originalFlags = self->field_008c;
    const uint32_t loadedFlags = originalFlags | 9;
    self->field_008c = originalFlags | 1u;
    self->field_008c = loadedFlags;
    self->field_008c = loadedFlags | 0x10;

    return result;
}

HOOK(0x006BF020, C_008605d8_LoadEnvironment, "auto");
