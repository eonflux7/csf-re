#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));
CSF_FUNC(void, __fastcall, C_00860a30_SetFlagAndOrBits, 0x006C9FB0, (C_00860a30* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __fastcall C_00862490_InitializeResource(C_00862490* self, void* /*edx*/)
{
    alignas(8) uint8_t normalizedNameBytes[0x100];
    alignas(8) uint8_t resourceRequest[0x24];
    char* normalizedName = reinterpret_cast<char*>(normalizedNameBytes);

    C_00860a30_SetFlagAndOrBits(self, nullptr);

    auto* resourceDefinition = reinterpret_cast<uint8_t*>(self->field_0018);
    char* resourceName;
    if (CSF_AT(uint32_t, resourceDefinition, 0x1C) < 0x10)
        resourceName = reinterpret_cast<char*>(resourceDefinition + 8);
    else
        resourceName = CSF_AT(char*, resourceDefinition, 0x08);

    NormalizeResourceName(resourceName, normalizedName, 256);

    resourceDefinition = reinterpret_cast<uint8_t*>(self->field_0018);
    CSF_AT(uint32_t, resourceRequest, 0x00) = 0;
    CSF_AT(uint32_t, resourceRequest, 0x1C) = 0;
    CSF_AT(uint32_t, resourceRequest, 0x18) = 0;
    CSF_AT(uint32_t, resourceRequest, 0x20) = 4;
    CSF_AT(uint32_t, resourceRequest, 0x04) = 0;
    CSF_AT(uint32_t, resourceRequest, 0x08) = 0;
    CSF_AT(uint32_t, resourceRequest, 0x0C) = 0;
    CSF_AT(float, resourceRequest, 0x10) = 1.0f;
    CSF_AT(float, resourceRequest, 0x14) = 1.0f;
    CSF_AT(uint32_t, resourceRequest, 0x18) =
        CSF_AT(uint32_t, resourceDefinition, 0x4C);
    CSF_AT(uint32_t, resourceRequest, 0x1C) =
        CSF_AT(uint32_t, resourceDefinition, 0x50);
    CSF_AT(char*, resourceRequest, 0x00) = normalizedName;
    CSF_AT(uint32_t, resourceRequest, 0x20) = 8;

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));

    self->field_0020 =
        ((uint32_t(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, registry, 0)[0x28 / 4])(
                registry, nullptr, resourceRequest);

    void* currentItem = reinterpret_cast<void*>(GetCurrentItemCount(self, nullptr));
    void* skin =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, currentItem, 0)[0x1C / 4])(
                currentItem, nullptr);

    int result = GfxSkin_SetKeyedValue(skin, nullptr, 6, 0);
    self->field_008c = -1;
    return result;
}

HOOK(0x006F1D70, C_00862490_InitializeResource, "auto");
