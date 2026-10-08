#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseTextureReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270,
         (const void* path));

CSF_GLOBAL(const char*, g_PrimaryTextureName, 0x008A889C);
CSF_GLOBAL(const char*, g_SecondaryTextureName, 0x008A88A0);

static int __fastcall C_00865a10_LoadTextures(
    C_00865a10* self, void* /*edx*/, int value, void* textureProvider)
{
    void* texture = reinterpret_cast<void*>(self->field_0050);
    if (texture)
        ReleaseTextureReference(static_cast<uint8_t*>(texture) + 4, nullptr);

    texture = reinterpret_cast<void*>(self->field_0054);
    if (texture)
        ReleaseTextureReference(static_cast<uint8_t*>(texture) + 4, nullptr);

    RwImageSetPath(CSF_STR(0x00841C64, "Menus/Textures/"));

    texture = ((void* (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, textureProvider, 0)[0x38 / 4])(
            textureProvider, nullptr, g_PrimaryTextureName);
    self->field_0050 = reinterpret_cast<uint32_t>(texture);
    if (texture)
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, texture, 0)[0x44 / 4])(texture, nullptr, 4);

    texture = ((void* (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, textureProvider, 0)[0x38 / 4])(
            textureProvider, nullptr, g_SecondaryTextureName);
    self->field_0054 = reinterpret_cast<uint32_t>(texture);
    if (texture)
    {
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, texture, 0)[0x44 / 4])(texture, nullptr, 4);

        texture = reinterpret_cast<void*>(self->field_0054);
        const void* dimensions = ((const void* (__fastcall*)(void*, void*))
            CSF_AT(void**, texture, 0)[0x40 / 4])(texture, nullptr);
        std::memcpy(&self->field_0058, dimensions, 4);
        std::memcpy(&self->field_005c,
                    static_cast<const uint8_t*>(dimensions) + 4, 4);
    }

    self->field_0048 = static_cast<uint32_t>(value);
    self->field_004c = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x00728A00, C_00865a10_LoadTextures, "auto");
