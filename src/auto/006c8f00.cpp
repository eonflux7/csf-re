#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));

static char __fastcall C_00860954_WriteSceneFile(
    C_00860954* self, void* /*edx*/, const char* filename)
{
    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x00840044, "w")), 0);
    if (!file)
        return 0;

    std::uint32_t length = 0;
    std::uint32_t byteOffset = 0;
    for (std::uint32_t index = 0; ; ++index)
    {
        const std::uint32_t begin = self->field_0008;
        if (!begin)
            break;

        const std::int32_t count =
            static_cast<std::int32_t>(self->field_000c - begin) / 36;
        if (index >= static_cast<std::uint32_t>(count))
            break;

        auto* record =
            reinterpret_cast<std::uint8_t*>(self->field_0008 + byteOffset);
        length = CSF_AT(std::uint32_t, record, 0x14);

        ((void (__fastcall*)(void*, void*, void*, std::uint32_t))
            CSF_AT(void**, file, 0)[0xC / 4])(file, nullptr, &length, 4);

        record = reinterpret_cast<std::uint8_t*>(
            self->field_0008 + byteOffset);
        void* text = CSF_AT(std::uint32_t, record, 0x18) < 0x10
            ? static_cast<void*>(record + 4)
            : CSF_AT(void*, record, 0x4);

        ((void (__fastcall*)(void*, void*, void*, std::uint32_t))
            CSF_AT(void**, file, 0)[0xC / 4])(file, nullptr, text, length);

        record = reinterpret_cast<std::uint8_t*>(
            self->field_0008 + byteOffset);
        ((void (__fastcall*)(void*, void*, void*, std::uint32_t))
            CSF_AT(void**, file, 0)[0xC / 4])(file, nullptr, record + 0x20, 2);

        byteOffset += 36;
    }

    length = 0;
    ((void (__fastcall*)(void*, void*, void*, std::uint32_t))
        CSF_AT(void**, file, 0)[0xC / 4])(file, nullptr, &length, 4);
    CloseSceneFile(file);
    return 1;
}

HOOK(0x006C8F00, C_00860954_WriteSceneFile, "auto");
