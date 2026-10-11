#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static bool Archive_PrepareField(bool* archive)
{
    const bool ready = CSF_AT(std::uint8_t, archive, 0) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
    }
    return ready;
}

static int __fastcall S_00405e60_Serialize(
    S_00405e60* self, void* /*edx*/, bool* archive)
{
    auto* fields = reinterpret_cast<std::uint8_t*>(self);

    bool failedToBegin = false;
    if (CSF_AT(std::uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        failedToBegin =
            !((bool(__fastcall*)(void*, void*, int, bool*))
                  CSF_AT(void**, serializer, 0)[0x80 / 4])(
                      serializer, nullptr, 0, archive);
    }

    const std::uint32_t savedContext =
        CSF_AT(std::uint32_t, archive, 0x1C);
    CSF_AT(std::uint8_t, archive, 0) = failedToBegin;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;

    if (!failedToBegin)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(fields + 4, archive);
    }

    if (Archive_PrepareField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, bool*))
                 CSF_AT(void**, serializer, 0)[0x54 / 4])(
                     serializer, nullptr, fields + 8, archive) == -1;
    }

    if (Archive_PrepareField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, bool*))
                 CSF_AT(void**, serializer, 0)[0x44 / 4])(
                     serializer, nullptr, fields + 0xD, archive) == -1;
    }

    if (Archive_PrepareField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, bool*))
                 CSF_AT(void**, serializer, 0)[0x54 / 4])(
                     serializer, nullptr, fields + 0x10, archive) == -1;
    }

    if (Archive_PrepareField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, bool*))
                 CSF_AT(void**, serializer, 0)[0x54 / 4])(
                     serializer, nullptr, fields + 0x14, archive) == -1;
    }

    if (Archive_PrepareField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, bool*))
                 CSF_AT(void**, serializer, 0)[0x44 / 4])(
                     serializer, nullptr, fields + 0x20, archive) == -1;
    }

    if (Archive_PrepareField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(std::uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, bool*))
                 CSF_AT(void**, serializer, 0)[0x44 / 4])(
                     serializer, nullptr, fields + 0x21, archive) == -1;
    }

    if (Archive_PrepareField(archive))
        SceneLoader_SerializeReference(fields + 0x24, archive);

    if (Archive_PrepareField(archive))
        SceneLoader_SerializeReference(fields + 0x28, archive);

    if (Archive_PrepareField(archive))
        SceneLoader_SerializeReference(fields + 0x2C, archive);

    if (Archive_PrepareField(archive))
        SceneLoader_SerializeReference(fields + 0x30, archive);

    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;

    if (CSF_AT(std::uint8_t, archive, 0) != 0)
    {
        CSF_AT(std::uint8_t, archive, 0) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, archive, 4);
    const bool completed =
        ((bool(__fastcall*)(void*, void*, bool*))
             CSF_AT(void**, serializer, 0)[0x7C / 4])(
                 serializer, nullptr, archive);

    if (!completed)
    {
        CSF_AT(std::uint8_t, archive, 0) = 1;
        return 1;
    }

    CSF_AT(std::uint8_t, archive, 0) = 0;
    return completed;
}

HOOK(0x00405E60, S_00405e60_Serialize, "auto");
