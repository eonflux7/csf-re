#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static CSF_FUNC(int, __fastcall, C_00867444_ResetState, 0x00739F10,
                (C_00867444* self, void* edx));
static CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350,
                (const char* filename, const char* mode, int flags));
static CSF_FUNC(void, __cdecl, CloseSceneFile, 0x00697980,
                (void* file));
static CSF_FUNC(char, __fastcall, LoadFontVersion1, 0x0073A680,
                (C_00867444* self, void* edx, void* file));
static CSF_FUNC(char, __fastcall, LoadFontVersion2, 0x0073AA60,
                (C_00867444* self, void* edx, void* file));
static CSF_FUNC(char, __fastcall, LoadFontVersion3, 0x0073AE60,
                (C_00867444* self, void* edx, void* file));

static char __fastcall C_00867444_LoadFontFile(
    C_00867444* self, void* /*edx*/)
{
    C_00867444_ResetState(self, nullptr);

    const char* filename;
    if (CSF_AT(std::uint32_t, self, 0x24) < 16u)
        filename = reinterpret_cast<const char*>(self) + 0x10;
    else
        filename = CSF_AT(const char*, self, 0x10);

    void* file = OpenSceneFile(filename, CSF_STR(0x0083E130, "r"), 0);
    if (!file)
        return 0;

    char header[16];
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, file, 0)[2])(file, nullptr, header, 16);

    const char* expectedHeader = CSF_STR(0x00867438, "FontWapa");
    for (int i = 0; i < 9; ++i)
    {
        if (header[i] != expectedHeader[i])
        {
            CloseSceneFile(file);
            return 0;
        }
    }

    int version = 0;
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, file, 0)[2])(file, nullptr, &version, 4);

    char result = 0;
    switch (version)
    {
    case 1:
        result = LoadFontVersion1(self, nullptr, file);
        break;
    case 2:
        result = LoadFontVersion2(self, nullptr, file);
        break;
    case 3:
        result = LoadFontVersion3(self, nullptr, file);
        break;
    }

    CloseSceneFile(file);
    return result;
}

HOOK(0x0073B290, C_00867444_LoadFontFile, "auto");
