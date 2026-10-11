#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Archive_SerializeFloatMember, 0x004076D0, (void* self, void* edx, int a2));

static int __fastcall Archive_SerializeComposite(void* self, void* /*edx*/, int a2)
{
    void* archive = reinterpret_cast<void*>(a2);
    auto* members = reinterpret_cast<std::uint8_t*>(self);
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, archive, 0x1C);

    bool failed = false;
    if (CSF_AT(std::uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        failed = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, a2);
    }
    CSF_AT(std::uint8_t, archive, 0) = failed;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, self, a2) == -1;
    }

    failed = CSF_AT(std::uint8_t, archive, 0) != 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, members + 8, a2) == -1;
    }

    failed = CSF_AT(std::uint8_t, archive, 0) != 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, members + 4, a2) == -1;
    }

    failed = CSF_AT(std::uint8_t, archive, 0) != 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeFloatMember(members + 12, nullptr, a2);
    }

    failed = CSF_AT(std::uint8_t, archive, 0) != 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeFloatMember(members + 28, nullptr, a2);
    }

    failed = CSF_AT(std::uint8_t, archive, 0) != 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        const bool result = ((bool(__fastcall*)(void*, void*, int))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, a2);
        if (result)
        {
            CSF_AT(std::uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004078D0, Archive_SerializeComposite, "auto");
