#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350, (const char* path, const char* mode, int flags));

static void SceneLoader_ReadBytes(void* file, void* destination,
                                  std::uint32_t size)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, std::uint32_t)>(
        CSF_AT(void**, file, 0)[8 / 4])(file, nullptr, destination, size);
}

static void SceneLoader_ReadString(void* file, void* destination,
                                   char* buffer, std::uint32_t& length)
{
    AssignStringRange_429B60(
        destination, nullptr, const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);
    SceneLoader_ReadBytes(file, &length, 4);
    if (length != 0)
    {
        SceneLoader_ReadBytes(file, buffer, length);
        buffer[length] = 0;
        AssignStringRange_429B60(
            destination, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));
    }
}

static char __stdcall SceneLoader_LoadSceneSettings(
    const char* path, C_0082d410* settings)
{
    char buffer[0x200] = {};
    std::uint32_t length = 0;
    std::uint32_t value = 0;

    void* file = OpenSceneFile(path, CSF_STR(0x0083E130, "r"), 0);
    if (!file)
        return 0;

    auto* data = reinterpret_cast<std::uint8_t*>(settings);
    SceneLoader_ReadString(file, settings, buffer, length);
    SceneLoader_ReadString(file, data + 0x1C, buffer, length);
    SceneLoader_ReadString(file, data + 0x38, buffer, length);
    SceneLoader_ReadString(file, data + 0x54, buffer, length);

    settings[9].field_0004 = 0;
    settings[9].field_0008 = 0;
    settings[10].vtbl = nullptr;

    SceneLoader_ReadBytes(file, &settings[9].field_0004, 4);
    SceneLoader_ReadBytes(file, &settings[9].field_0008, 4);
    SceneLoader_ReadBytes(file, &settings[10].vtbl, 4);

    SceneLoader_ReadBytes(file, &value, 4);
    reinterpret_cast<std::uint8_t*>(&settings[10].field_0004)[1] =
        value == 1;

    SceneLoader_ReadBytes(file, &value, 4);
    settings[10].field_0008 = value;

    SceneLoader_ReadBytes(file, &value, 4);
    settings[11].vtbl = reinterpret_cast<void**>(value);

    SceneLoader_ReadBytes(file, &value, 4);
    settings[11].field_0004 = value;

    SceneLoader_ReadBytes(file, &value, 4);
    reinterpret_cast<std::uint8_t*>(&settings[11].field_0008)[0] =
        value == 1;

    SceneLoader_ReadBytes(file, &value, 4);
    reinterpret_cast<std::uint8_t*>(&settings[10].field_0004)[0] =
        value == 1;

    CloseSceneFile(file);
    return 1;
}

HOOK(0x00501450, SceneLoader_LoadSceneSettings, "auto");
