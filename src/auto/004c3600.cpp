#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall S_00523cf0_SerializeFields(
    S_00523cf0* self, void* /*edx*/, S_00691860* archive)
{
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    auto* fields = reinterpret_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0) {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archive);
    }
    CSF_AT(uint8_t, archive, 0x00) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed) {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, fields, archive) == -1;
    }

    bool processNext = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processNext) {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, fields + 0x04, archive) == -1;
    }

    processNext = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processNext) {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, fields + 0x08, archive) == -1;
    }

    processNext = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processNext) {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(fields + 0x0C), archive);
    }

    processNext = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processNext) {
        void* backend = reinterpret_cast<void*>(archive->field_0004);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, fields + 0x18, archive) == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0x00) != 0) {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* backend = reinterpret_cast<void*>(archive->field_0004);
    const int result =
        ((int(__fastcall*)(void*, void*, S_00691860*))
            CSF_AT(void**, backend, 0)[0x7C / 4])(
                backend, nullptr, archive);
    if (static_cast<uint8_t>(result) == 0) {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x004C3600, S_00523cf0_SerializeFields, "auto");
