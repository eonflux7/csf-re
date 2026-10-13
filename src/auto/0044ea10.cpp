#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeMemberCollection(
    void* self, void* /*edx*/, S_00691860* archive)
{
    auto* members = reinterpret_cast<std::uint8_t*>(self);

    bool failed = false;
    if (!archive->pad_0000[0])
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, archive);
    }

    const std::uint32_t savedContext = CSF_AT(std::uint32_t, archive, 0x1C);
    archive->pad_0000[0] = failed;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x58 / 4])(
                    stream, nullptr, members, archive) == -1;
    }

    bool proceed = archive->pad_0000[0] == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x58 / 4])(
                    stream, nullptr, members + 4, archive) == -1;
    }

    proceed = archive->pad_0000[0] == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(members + 8), archive);
    }

    proceed = archive->pad_0000[0] == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x38 / 4])(
                    stream, nullptr, members + 20, archive) == -1;
    }

    proceed = archive->pad_0000[0] == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, members + 32, archive) == -1;
    }

    proceed = archive->pad_0000[0] == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, members + 33, archive) == -1;
    }

    proceed = archive->pad_0000[0] == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, members + 34, archive) == -1;
    }

    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;
    if (!archive->pad_0000[0])
    {
        void* stream = reinterpret_cast<void*>(archive->field_0004);
        const int result =
            ((int(__fastcall*)(void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, archive);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            archive->pad_0000[0] = 0;
            return result;
        }
    }

    archive->pad_0000[0] = 1;
    return 1;
}

HOOK(0x0044EA10, Archive_SerializeMemberCollection, "auto");
