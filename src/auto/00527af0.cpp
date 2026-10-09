#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

static int __fastcall S_00526d40_SerializeSoundProperties(
    S_00526d40* self, void* /*edx*/, S_00691860* archive)
{
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    auto* properties = reinterpret_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archive);
    }
    CSF_AT(uint8_t, archive, 0x00) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_0000, archive) == -1;
    }

    bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, properties + 0x04, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, properties + 0x08, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, properties + 0x10, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, properties + 0x14, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, properties + 0x18, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(properties + 0x24), archive);
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, properties + 0x36, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, properties + 0x34, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, properties + 0x35, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, properties + 0x30, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, properties + 0x0C, archive) == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0x00) != 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* backend = reinterpret_cast<void*>(archive->field_0004);
    const int result =
        ((int(__fastcall*)(void*, void*, S_00691860*))
            CSF_AT(void**, backend, 0)[0x7C / 4])(backend, nullptr, archive);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x00527AF0, S_00526d40_SerializeSoundProperties, "auto");
