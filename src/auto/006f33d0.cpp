#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void, __fastcall, C_00860a30_SetFlagAndOrBits, 0x006C9FB0, (C_00860a30* self, void*));
CSF_FUNC(int, __fastcall, GfxRenderEngine_SetResourceRectangle, 0x006EECB0, (S_006ada10* self, void* , uint32_t* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __fastcall C_00862640_LoadResource(C_00862640* self, void* /*edx*/)
{
    C_00860a30_SetFlagAndOrBits(self, nullptr);

    auto* definition = reinterpret_cast<uint8_t*>(self->field_0018);
    char* sourceName;
    if (CSF_AT(uint32_t, definition, 0x1C) < 0x10)
        sourceName = reinterpret_cast<char*>(definition + 8);
    else
        sourceName = CSF_AT(char*, definition, 0x08);

    alignas(8) uint8_t nameBytes[0x100];
    auto* normalizedName = reinterpret_cast<char*>(nameBytes);
    NormalizeResourceName(sourceName, normalizedName, 256);

    definition = reinterpret_cast<uint8_t*>(self->field_0018);

    alignas(8) uint8_t requestBytes[0x24];
    CSF_AT(uint32_t, requestBytes, 0x00) = 0;
    CSF_AT(uint32_t, requestBytes, 0x1C) = 0;
    CSF_AT(uint32_t, requestBytes, 0x18) = 0;
    CSF_AT(uint32_t, requestBytes, 0x20) = 4;
    CSF_AT(uint32_t, requestBytes, 0x04) = 0;
    CSF_AT(uint32_t, requestBytes, 0x08) = 0;
    CSF_AT(uint32_t, requestBytes, 0x0C) = 0;
    CSF_AT(float, requestBytes, 0x10) = 1.0f;
    CSF_AT(float, requestBytes, 0x14) = 1.0f;

    CSF_AT(uint32_t, requestBytes, 0x18) = CSF_AT(uint32_t, definition, 0x4C);
    CSF_AT(uint32_t, requestBytes, 0x1C) = CSF_AT(uint32_t, definition, 0x50);
    CSF_AT(char*, requestBytes, 0x00) = normalizedName;
    CSF_AT(uint32_t, requestBytes, 0x20) = 0;

    uint32_t flags = CSF_AT(uint8_t, definition, 0x80) != 0 ? 8 : 0;
    CSF_AT(uint32_t, requestBytes, 0x20) = flags;
    const uint32_t mode = CSF_AT(uint32_t, definition, 0x58);
    if (mode == 0)
        CSF_AT(uint32_t, requestBytes, 0x20) = flags | 0x10;
    else if (mode == 2)
        CSF_AT(uint32_t, requestBytes, 0x20) = flags | 0x20;

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
    auto* resource =
        reinterpret_cast<S_006ada10* (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, registry, 0)[0x28 / 4])(
                registry, nullptr, reinterpret_cast<uint32_t*>(requestBytes));

    self->field_0020 = reinterpret_cast<uint32_t>(resource);
    definition = reinterpret_cast<uint8_t*>(self->field_0018);

    alignas(8) uint8_t rectangleBytes[0x10];
    CSF_AT(uint32_t, rectangleBytes, 0x00) = 0;
    CSF_AT(uint32_t, rectangleBytes, 0x04) = 0;
    CSF_AT(float, rectangleBytes, 0x08) =
        g_TransitionOne / static_cast<float>(CSF_AT(int32_t, definition, 0x74));
    CSF_AT(float, rectangleBytes, 0x0C) =
        g_TransitionOne / static_cast<float>(CSF_AT(int32_t, definition, 0x78));

    return GfxRenderEngine_SetResourceRectangle(
        resource, nullptr, reinterpret_cast<uint32_t*>(rectangleBytes));
}

HOOK(0x006F33D0, C_00862640_LoadResource, "auto");
