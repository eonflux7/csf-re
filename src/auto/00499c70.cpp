#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

// Serializes a small record: [block begin] object header, name string at +4, value at +0x10 [block end].
// Archive layout used here:
//   +0x00 byte  error flag
//   +0x04 ptr   stream/serializer implementation (virtual)
//   +0x14 byte  "field present" flag, +0x15/+0x16 per-field flags
//   +0x1C dword current field name / context
static int __fastcall NamedEntry_Serialize(void* self, void* /*edx*/, S_00691860* archive)
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
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int status = ((int(__fastcall*)(void*, void*, void*, S_00691860*))CSF_AT(void**, stream, 0)[0x58 / 4])(
            stream, nullptr, self, archive);
        CSF_AT(uint8_t, archive, 0x0) = (status == -1) ? 1 : 0;
    }

    bool ok = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ok)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString((int)((uint8_t*)self + 4), archive);
    }

    ok = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ok)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int status = ((int(__fastcall*)(void*, void*, void*, S_00691860*))CSF_AT(void**, stream, 0)[0x48 / 4])(
            stream, nullptr, (uint8_t*)self + 0x10, archive);
        CSF_AT(uint8_t, archive, 0x0) = (status == -1) ? 1 : 0;
    }

    // Field was not present in the stream: default value.
    if (CSF_AT(uint8_t, archive, 0x14) == 0)
        CSF_AT(int32_t, self, 0x14) = -1;

    ok = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (ok)
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

HOOK(0x00499C70, NamedEntry_Serialize, "auto");
