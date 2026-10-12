#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Archive_SerializeEmbeddedData, 0x0040C3C0,
         (void* a1, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10,
         (void* a3, bool* a2));

static int __fastcall C_0082daf4_Serialize(
    C_0082daf4* self, void* /*edx*/, int archiveAddress)
{
    auto* archive = reinterpret_cast<uint8_t*>(archiveAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archiveAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            &self->field_0004, reinterpret_cast<bool*>(archive));
    }

    bool canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_0008, archiveAddress);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeEmbeddedData(
            reinterpret_cast<uint8_t*>(self) + 0x0C, archiveAddress);
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeEmbeddedData(
            reinterpret_cast<uint8_t*>(self) + 0x1C, archiveAddress);
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (canContinue)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        const int result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, archiveAddress);
        if (result)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x0040C5B0, C_0082daf4_Serialize, "auto");
