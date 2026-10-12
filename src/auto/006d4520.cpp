#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));

static bool __fastcall GfxClumpDict_WriteScenePathList(
    void* self, void* /*edx*/, const char* filename)
{
    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x00840044, "w")), 0);
    if (!file)
        return false;

    std::int32_t length = 0;
    std::uint32_t byteOffset = 0;
    for (std::uint32_t index = 0; ; ++index, byteOffset += 60)
    {
        const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x08);
        if (!begin)
            break;

        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x0C) - begin);
        if (index >= static_cast<std::uint32_t>(byteCount / 60))
            break;

        auto* entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x08)) + byteOffset;
        length = CSF_AT(std::int32_t, entry, 0x14);
        reinterpret_cast<void(__fastcall*)(void*, void*, const void*, int)>(
            CSF_AT(void**, file, 0)[3])(file, nullptr, &length, 4);

        entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x08)) + byteOffset;
        const void* text;
        if (CSF_AT(std::uint32_t, entry, 0x18) < 0x10)
            text = entry + 4;
        else
            text = CSF_AT(void*, entry, 0x04);

        reinterpret_cast<void(__fastcall*)(void*, void*, const void*, int)>(
            CSF_AT(void**, file, 0)[3])(file, nullptr, text, length);

        entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x08)) + byteOffset;
        reinterpret_cast<void(__fastcall*)(void*, void*, const void*, int)>(
            CSF_AT(void**, file, 0)[3])(file, nullptr, entry + 0x1C, 4);
    }

    length = 0;
    reinterpret_cast<void(__fastcall*)(void*, void*, const void*, int)>(
        CSF_AT(void**, file, 0)[3])(file, nullptr, &length, 4);
    CloseSceneFile(file);
    return true;
}

HOOK(0x006D4520, GfxClumpDict_WriteScenePathList, "auto");
