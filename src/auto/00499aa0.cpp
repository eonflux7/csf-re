#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

// Serializes a record made of two strings (+0x00, +0x0C) and two scalar fields (+0x18, +0x1C)
// through the archive's stream object, wrapped in a begin/end block.
static int __fastcall Archive_SerializeTwoStringRecord(void* self, void* /*edx*/, S_00691860* archive)
{
    uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        bool began = ((bool(__fastcall*)(void*, void*, int, S_00691860*))CSF_AT(void**, stream, 0)[0x80 / 4])(
            stream, nullptr, 0, archive);
        if (!began)
            failed = true;
    }
    CSF_AT(uint8_t, archive, 0x0) = failed ? 1 : 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString((int)(uintptr_t)self, archive);
    }

    bool ok = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ok)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString((int)(uintptr_t)((uint8_t*)self + 0xC), archive);
    }

    ok = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ok)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int status = ((int(__fastcall*)(void*, void*, void*, S_00691860*))CSF_AT(void**, stream, 0)[0x54 / 4])(
            stream, nullptr, (uint8_t*)self + 0x18, archive);
        CSF_AT(uint8_t, archive, 0x0) = (status == -1) ? 1 : 0;
    }

    ok = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ok)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int status = ((int(__fastcall*)(void*, void*, void*, S_00691860*))CSF_AT(void**, stream, 0)[0x38 / 4])(
            stream, nullptr, (uint8_t*)self + 0x1C, archive);
        CSF_AT(uint8_t, archive, 0x0) = (status == -1) ? 1 : 0;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;

    if (CSF_AT(uint8_t, archive, 0x0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        int endResult = ((int(__fastcall*)(void*, void*, S_00691860*))CSF_AT(void**, stream, 0)[0x7C / 4])(
            stream, nullptr, archive);
        if ((uint8_t)endResult != 0)
        {
            CSF_AT(uint8_t, archive, 0x0) = 0;
            return endResult;
        }
    }

    CSF_AT(uint8_t, archive, 0x0) = 1;
    return 1;
}

HOOK(0x00499AA0, Archive_SerializeTwoStringRecord, "auto");
