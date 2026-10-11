#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (void* a1, int a2));
CSF_FUNC(unsigned int, __stdcall, sub_447990, 0x00447990, (void* a1, int a2));
CSF_FUNC(unsigned int, __stdcall, sub_447B80, 0x00447B80, (void* a1, int a2));
CSF_FUNC(int, __stdcall, sub_447D70, 0x00447D70, (uint32_t* a1, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_00831120_Serialize(
    C_00831120* self, void* /*edx*/, int archiveAddress)
{
    auto* archive = reinterpret_cast<uint8_t*>(archiveAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, archiveAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1c);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1c) = 0;

    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, &self->field_0008, archiveAddress);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    bool active = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1c) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            &self->field_0114, reinterpret_cast<bool*>(archive));
    }

    active = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1c) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        sub_447990(&self->field_000c, archiveAddress);
    }

    active = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1c) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        sub_447B80(&self->field_001c, archiveAddress);
    }

    active = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1c) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        sub_447D70(&self->field_002c, archiveAddress);
    }

    active = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1c) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeRecordMember(&self->field_0040, archiveAddress);
    }

    active = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1c) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeRecordMember(&self->field_0050, archiveAddress);
    }

    active = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1c) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeRecordMember(
            &CSF_AT(uint8_t, self, 0xc8), archiveAddress);
    }

    active = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1c) = savedContext;
    if (active)
    {
        void* stream = CSF_AT(void*, archive, 4);
        const bool completed =
            reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0)[0x7c / 4])(
                    stream, nullptr, archiveAddress);
        if (completed)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return completed;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x00447F70, C_00831120_Serialize, "auto");
