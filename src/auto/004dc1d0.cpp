#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

static int __fastcall C_0083def8_Serialize(
    C_0083def8* self, void* /*edx*/, bool* archive)
{
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 4);
        failed = !((bool(__fastcall*)(void*, void*, int, bool*))
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, archive);
    }

    uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0004, archive);
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0008, archive);
    }

    char hasChild = self->field_000c != 0;
    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int status = ((int(__fastcall*)(void*, void*, char*, bool*))
            CSF_AT(void**, stream, 0)[0x44 / 4])(
                stream, nullptr, &hasChild, archive);
        CSF_AT(uint8_t, archive, 0) = status == -1;
    }

    if (hasChild)
    {
        if (CSF_AT(uint8_t, archive, 0x14) == 0)
        {
            auto* child = reinterpret_cast<C_0083def8*>(
                CoreMemory_AllocUntagged(20));
            if (child)
            {
                child->vtbl = CSF_VTBL(0x0083DEF8);
                child->field_0004 = 0;
                child->field_0008 = 0;
                child->field_000c = 0;
                CSF_AT(uint32_t, child, 0x10) = 0;
            }
            self->field_000c = reinterpret_cast<uint32_t>(child);
        }

        failed = CSF_AT(uint8_t, archive, 0) != 0;
        CSF_AT(uint32_t, archive, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(uint8_t, archive, 0x15) = 1;
            CSF_AT(uint8_t, archive, 0x16) = 0;
            Archive_SerializeObjectRef(&self->field_000c, archive);
        }
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            reinterpret_cast<uint8_t*>(self) + 0x10, archive);
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 4);
        int result = ((int(__fastcall*)(void*, void*, bool*))
            CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004DC1D0, C_0083def8_Serialize, "auto");
