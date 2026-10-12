#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));

CSF_GLOBAL(std::uint32_t, g_ClumpFileHeader, 0x00860C80);
CSF_GLOBAL(std::uint32_t, g_ClumpFileVersion, 0x00860C84);

static void GfxCollisionShapes_WriteSceneData(void* file, void* data, int size)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, file, 0)[3])(file, nullptr, data, size);
}

static char __fastcall GfxCollisionShapes_SaveClumpFile(
    void* self, void* /*edx*/, const char* filename)
{
    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x00840044, "w")), 0);
    if (!file)
        return 0;

    GfxCollisionShapes_WriteSceneData(file, &g_ClumpFileHeader, 4);
    GfxCollisionShapes_WriteSceneData(file, &g_ClumpFileVersion, 4);

    int length = 0;
    if (CSF_AT(std::uint32_t, self, 0x08))
    {
        length = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x0C) -
            CSF_AT(std::uint32_t, self, 0x08)) / 100;
    }
    GfxCollisionShapes_WriteSceneData(file, &length, 4);

    std::uint32_t index = 0;
    std::uint32_t byteOffset = 0;
    for (;;)
    {
        const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x08);
        if (!begin)
            break;

        const int count = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x0C) - begin) / 100;
        if (index >= static_cast<std::uint32_t>(count))
            break;

        auto* entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x08) + byteOffset);

        GfxCollisionShapes_WriteSceneData(file, entry, 4);

        length = CSF_AT(std::int32_t, entry, 0x18);
        GfxCollisionShapes_WriteSceneData(file, &length, 4);
        if (length > 0)
        {
            void* text = CSF_AT(std::uint32_t, entry, 0x1C) < 0x10u
                ? static_cast<void*>(entry + 0x08)
                : reinterpret_cast<void*>(CSF_AT(std::uint32_t, entry, 0x08));
            GfxCollisionShapes_WriteSceneData(file, text, length);
        }

        length = CSF_AT(std::int32_t, entry, 0x34);
        GfxCollisionShapes_WriteSceneData(file, &length, 4);
        if (length > 0)
        {
            void* text = CSF_AT(std::uint32_t, entry, 0x38) < 0x10u
                ? static_cast<void*>(entry + 0x24)
                : reinterpret_cast<void*>(CSF_AT(std::uint32_t, entry, 0x24));
            GfxCollisionShapes_WriteSceneData(file, text, length);
        }

        GfxCollisionShapes_WriteSceneData(file, entry + 0x48, 4);
        GfxCollisionShapes_WriteSceneData(file, entry + 0x4C, 4);
        GfxCollisionShapes_WriteSceneData(file, entry + 0x50, 4);
        GfxCollisionShapes_WriteSceneData(file, entry + 0x3C, 4);
        GfxCollisionShapes_WriteSceneData(file, entry + 0x40, 4);
        GfxCollisionShapes_WriteSceneData(file, entry + 0x44, 4);
        GfxCollisionShapes_WriteSceneData(file, entry + 0x54, 4);
        GfxCollisionShapes_WriteSceneData(file, entry + 0x58, 4);
        GfxCollisionShapes_WriteSceneData(file, entry + 0x5C, 4);

        ++index;
        byteOffset += 100;
    }

    CloseSceneFile(file);
    return 1;
}

HOOK(0x006CDFB0, GfxCollisionShapes_SaveClumpFile, "auto");
