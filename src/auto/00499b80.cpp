#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

// Serializes a record { int @+4, string @+8, int @+0x14, value @+0x18 } through an archive.
// archive+0x00: error flag, +0x04: stream object (virtual), +0x15/+0x16: per-field flags,
// +0x1C: current field descriptor (saved/cleared/restored around the block).
static int __fastcall Archive_SerializeNamedRecord(void* self, void* /*edx*/, void* archive)
{
    uint32_t savedFieldInfo = CSF_AT(uint32_t, archive, 0x1C);

    bool failed;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        bool began = ((bool(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, stream, 0)[0x80 / 4])(
            stream, nullptr, 0, archive);
        failed = !began;
    }
    else
    {
        failed = false;
    }
    CSF_AT(uint8_t, archive, 0x00) = failed ? 1 : 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int rc = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x58 / 4])(
            stream, nullptr, (uint8_t*)self + 4, archive);
        CSF_AT(uint8_t, archive, 0x00) = (rc == -1) ? 1 : 0;
    }

    // The error flag is read before the field descriptor is cleared.
    bool stringOk = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (stringOk)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString((int)((uint8_t*)self + 8), archive);
    }

    bool secondIntOk = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (secondIntOk)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int rc = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x58 / 4])(
            stream, nullptr, (uint8_t*)self + 0x14, archive);
        CSF_AT(uint8_t, archive, 0x00) = (rc == -1) ? 1 : 0;
    }

    bool lastFieldOk = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (lastFieldOk)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int rc = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x38 / 4])(
            stream, nullptr, (uint8_t*)self + 0x18, archive);
        CSF_AT(uint8_t, archive, 0x00) = (rc == -1) ? 1 : 0;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedFieldInfo;

    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        int endResult = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, stream, 0)[0x7C / 4])(
            stream, nullptr, archive);
        if ((uint8_t)endResult != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return endResult;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x00499B80, Archive_SerializeNamedRecord, "auto");
