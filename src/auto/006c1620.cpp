#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(int, __cdecl, RwImageGetPath, 0x00625320, ());
CSF_FUNC(int, __cdecl, RwTextureRead, 0x006319C0, (int a1, int a2));
CSF_FUNC(char*, __cdecl, GfxTexdict_ExtractResourceExtension, 0x00694860, (char* a1, uint8_t* a2, char* a3));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

static int __stdcall GfxTexdict_ReadTextureFromResource(char* source)
{
    if (!source || !*source)
        return 0;

    alignas(8) char savedPath[512];
    alignas(8) uint8_t resourcePath[256];
    alignas(8) char resourceName[32];
    alignas(8) uint8_t extension[8];

    Hud_ExtractResourcePath(source, resourcePath, 256);
    NormalizeResourceName(source, resourceName, 32);
    GfxTexdict_ExtractResourceExtension(
        source, extension, reinterpret_cast<char*>(8));

    if (std::strlen(reinterpret_cast<const char*>(resourcePath)) == 0)
        return RwTextureRead(reinterpret_cast<int>(resourceName), 0) != 0;

    const char* currentPath = reinterpret_cast<const char*>(RwImageGetPath());
    _strncpy(savedPath, currentPath, 0x200u);
    savedPath[511] = 0;

    RwImageSetPath(resourcePath);
    int texture = RwTextureRead(reinterpret_cast<int>(resourceName), 0);
    RwImageSetPath(savedPath);
    return texture != 0;
}

HOOK(0x006C1620, GfxTexdict_ReadTextureFromResource, "auto");
