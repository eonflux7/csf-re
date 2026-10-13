#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));

static int __cdecl CoreFilesystemRtfs_OpenFile(
    int /*manager*/, int* state, const char* filename, char flags)
{
    if ((flags & 0x10) != 0)
        return 3;

    CSF_AT(int, state, 0x18) = 0;
    CSF_AT(int, state, 0x20) = 5;
    CSF_AT(int, state, 0x08) = 0;
    CSF_AT(int, state, 0x0C) = 0;
    CSF_AT(int, state, 0x00) = 0;
    CSF_AT(int, state, 0x04) = 0;

    if ((flags & 1) != 0)
    {
        CSF_AT(void*, state, 0x40) = CoreFiles_OpenSceneFile(
            filename, const_cast<char*>(CSF_STR(0x008579F8, "rb")), 0);
    }
    else if ((flags & 8) != 0)
    {
        CSF_AT(void*, state, 0x40) = CoreFiles_OpenSceneFile(
            filename, const_cast<char*>(CSF_STR(0x00857A00, "ab")), 0);
    }
    else if ((flags & 2) != 0)
    {
        CSF_AT(void*, state, 0x40) = CoreFiles_OpenSceneFile(
            filename, const_cast<char*>(CSF_STR(0x008579FC, "wb")), 0);
    }

    void* file = CSF_AT(void*, state, 0x40);
    if (file)
    {
        int initialValue =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, file, 0)[0x1C / 4])(file, nullptr);

        CSF_AT(int, state, 0x04) = 0;
        file = CSF_AT(void*, state, 0x40);
        CSF_AT(int, state, 0x00) = initialValue;

        CSF_AT(int, state, 0x08) =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, file, 0)[0x10 / 4])(file, nullptr);
        CSF_AT(int, state, 0x0C) = 0;
    }

    const bool failed = CSF_AT(void*, state, 0x40) == nullptr;
    CSF_AT(int, state, 0x20) = 2;
    return 2 * failed + 1;
}

HOOK(0x0069FDD0, CoreFilesystemRtfs_OpenFile, "auto");
