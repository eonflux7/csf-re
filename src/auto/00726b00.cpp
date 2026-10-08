#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0,
         (char* source, char* destination, unsigned int capacity));
CSF_FUNC(void*, __cdecl, FindNamedTexture, 0x00631950,
         (void* dictionary, char* name));
CSF_FUNC(void*, __cdecl, TryLoadTexture, 0x006E2ED0,
         (char* name, char* status));
CSF_FUNC(void*, __cdecl, LoadTextureWithOptions, 0x006E2EB0,
         (char* name, char option, char* status));
CSF_FUNC(int, __cdecl, SetTextureName, 0x00631800,
         (void* texture, char* name));
CSF_FUNC(int, __cdecl, AddDictionaryTexture, 0x006318E0,
         (void* dictionary, void* texture));
CSF_FUNC(int, __cdecl, GetDictionaryStreamSize, 0x006307D0,
         (void* dictionary));

static int __fastcall C_008655e8_AcquireTexture(
    C_008655e8* self, void* /*edx*/, char* source)
{
    if (!source || !*source)
        return 0;

    char normalizedName[256] = {};
    NormalizeResourceName(source, normalizedName, 256);

    void* texture = FindNamedTexture(
        reinterpret_cast<void*>(self->field_002c), normalizedName);

    if (texture)
    {
        ++CSF_AT(std::uint32_t, texture, 0x54);
        return reinterpret_cast<std::intptr_t>(texture);
    }

    // The original uses the high byte of its source argument as output storage.
    char* status = reinterpret_cast<char*>(&source) + 3;
    *status = 0;

    texture = TryLoadTexture(normalizedName, status);
    if (!texture)
        texture = LoadTextureWithOptions(normalizedName, 0, status);

    if (texture)
    {
        SetTextureName(texture, normalizedName);
        AddDictionaryTexture(
            reinterpret_cast<void*>(self->field_002c), texture);
        self->field_0030 = GetDictionaryStreamSize(
            reinterpret_cast<void*>(self->field_002c));
    }

    return reinterpret_cast<std::intptr_t>(texture);
}

HOOK(0x00726B00, C_008655e8_AcquireTexture, "auto");
