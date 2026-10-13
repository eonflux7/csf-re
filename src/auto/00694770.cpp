#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, MenuExtras_OpenFileDescriptor, 0x00694660, (void* self, void* edx, int a2, uint32_t a3, char* a4, uint32_t* a5));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));

static char __fastcall S_00693740_LoadFile(
    S_00693740* self, void* /*edx*/, int filename)
{
    void* file = CoreFiles_OpenSceneFile(
        reinterpret_cast<const char*>(filename),
        const_cast<char*>(CSF_STR(0x0083E130, "r")),
        0);

    if (!file)
    {
        CloseSceneFile(file);
        return 0;
    }

    uint32_t fileSize =
        ((uint32_t(__fastcall*)(void*, void*))
            CSF_AT(void**, file, 0)[0x1C / 4])(file, nullptr);

    char opened = MenuExtras_OpenFileDescriptor(
        self, nullptr, reinterpret_cast<int>(file), fileSize, nullptr,
        reinterpret_cast<uint32_t*>(&filename));

    CloseSceneFile(file);
    return opened ? 1 : 0;
}

HOOK(0x00694770, S_00693740_LoadFile, "auto");
