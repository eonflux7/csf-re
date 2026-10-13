#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));
CSF_FUNC(void, __fastcall, C_00860a30_SetFlagAndOrBits, 0x006C9FB0, (C_00860a30* self, void*));
CSF_FUNC(int, __fastcall, GfxRenderEngine_SetResourceRectangle, 0x006EECB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, GfxRenderEngine_CreateResource, 0x006EF3C0, ());

static int __fastcall C_00861508_InitializeNamedResource(
    C_00861508* self, void* /*edx*/)
{
    C_00860a30_SetFlagAndOrBits(
        reinterpret_cast<C_00860a30*>(self), nullptr);

    void* currentItem = reinterpret_cast<void*>(GetCurrentItemCount(self, nullptr));
    int skin = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, currentItem, 0)[0x1C / 4])(currentItem, nullptr);

    GfxSkin_SetKeyedValue(reinterpret_cast<void*>(skin), nullptr, 11, 2);
    self->field_0094 =
        reinterpret_cast<uint32_t>(GfxRenderEngine_CreateResource());

    auto* resourceName =
        reinterpret_cast<uint8_t*>(self->field_0018) + 4;

    alignas(8) uint8_t destinationBytes[0x100];
    alignas(8) uint8_t rectangleBytes[0x10];
    alignas(8) uint8_t descriptorBytes[0x24];

    CSF_AT(uint32_t, descriptorBytes, 0x00) = 0;
    CSF_AT(uint32_t, descriptorBytes, 0x04) = 0;
    CSF_AT(uint32_t, descriptorBytes, 0x08) = 0;
    CSF_AT(uint32_t, descriptorBytes, 0x0C) = 0;
    CSF_AT(float, descriptorBytes, 0x10) = 1.0f;
    CSF_AT(float, descriptorBytes, 0x14) = 1.0f;
    CSF_AT(float, descriptorBytes, 0x1C) = 0.0f;
    CSF_AT(float, descriptorBytes, 0x18) = 0.0f;
    CSF_AT(uint32_t, descriptorBytes, 0x20) = 4;

    char* source;
    if (CSF_AT(uint32_t, resourceName, 0x18) < 0x10u)
        source = reinterpret_cast<char*>(resourceName + 4);
    else
        source = CSF_AT(char*, resourceName, 0x04);

    NormalizeResourceName(
        source, reinterpret_cast<char*>(destinationBytes), 256);

    void* resource = reinterpret_cast<void*>(self->field_0094);
    CSF_AT(char*, descriptorBytes, 0x00) =
        reinterpret_cast<char*>(destinationBytes);
    CSF_AT(uint32_t, descriptorBytes, 0x20) = 44;
    CSF_AT(float, descriptorBytes, 0x1C) = 1.0f;
    CSF_AT(float, descriptorBytes, 0x18) = 1.0f;

    bool loaded = ((bool(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, resource, 0)[0x10 / 4])(
            resource, nullptr, reinterpret_cast<uint32_t*>(descriptorBytes));

    resource = reinterpret_cast<void*>(self->field_0094);
    if (loaded)
    {
        CSF_AT(float, rectangleBytes, 0x00) = 0.0f;
        CSF_AT(float, rectangleBytes, 0x04) = 0.0f;
        CSF_AT(float, rectangleBytes, 0x08) = 1.0f;
        CSF_AT(float, rectangleBytes, 0x0C) = 1.0f;
        return GfxRenderEngine_SetResourceRectangle(
            resource, nullptr, reinterpret_cast<uint32_t*>(rectangleBytes));
    }

    int result = 0;
    if (resource)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, resource, 0)[0x04 / 4])(resource, nullptr, 1);
        self->field_0094 = 0;
    }
    return result;
}

HOOK(0x006DCDD0, C_00861508_InitializeNamedResource, "auto");
