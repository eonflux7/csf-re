#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, S_0040c070_SerializeElements, 0x0040C3C0, (S_0040c070* self, int contextAddress));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_0082daf4_SerializeReferencesAndMembers(
    C_0082daf4* self, void* /*edx*/, int contextAddress)
{
    void* archive = reinterpret_cast<void*>(contextAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, contextAddress);
    }

    int savedContext = CSF_AT(int, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    CSF_AT(int, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            &self->field_0004, reinterpret_cast<bool*>(archive));
    }

    bool canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int result = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
            CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, &self->field_0008, contextAddress);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        S_0040c070_SerializeElements(
            reinterpret_cast<S_0040c070*>(
                reinterpret_cast<uint8_t*>(self) + 0x0C),
            contextAddress);
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        S_0040c070_SerializeElements(
            reinterpret_cast<S_0040c070*>(
                reinterpret_cast<uint8_t*>(self) + 0x1C),
            contextAddress);
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = savedContext;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x0040C5B0, C_0082daf4_SerializeReferencesAndMembers, "auto");
