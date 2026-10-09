#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Archive helpers (cdecl): serialize a member through the archive
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeMissionSetupMember, 0x004E9200, (void* a3, bool* a2));

// Serializes the two members at +0x20 and +0x1C of the object inside an archive chunk.
// archive layout: +0x00 error flag, +0x04 stream (virtual), +0x15/+0x16 mode flags, +0x1C current context.
static int __fastcall SceneMissionSetup_Serialize(void* self, void* /*edx*/, bool* archive)
{
    bool failed;
    if (CSF_AT(uint8_t, archive, 0x0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        bool opened = ((bool(__fastcall*)(void*, void*, int, bool*))CSF_AT(void**, stream, 0)[0x80 / 4])(
            stream, nullptr, 0, archive);
        failed = !opened ? true : false;
    }
    else
    {
        failed = false;
    }

    uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x0) = failed ? 1 : 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeMissionSetupMember((uint8_t*)self + 0x20, archive);
    }

    bool stillOk = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (stillOk)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef((uint8_t*)self + 0x1C, archive);
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;

    if (CSF_AT(uint8_t, archive, 0x0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        bool closed = ((bool(__fastcall*)(void*, void*, bool*))CSF_AT(void**, stream, 0)[0x7C / 4])(
            stream, nullptr, archive);
        if (closed)
        {
            CSF_AT(uint8_t, archive, 0x0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, archive, 0x0) = 1;
    return 1;
}

HOOK(0x004E9E10, SceneMissionSetup_Serialize, "auto");
