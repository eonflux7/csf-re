#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Archive_SerializeFirstCollection, 0x0048D1B0, (int a1, int a2));
CSF_FUNC(int, __stdcall, Archive_SerializeSecondCollection, 0x0048D300, (unsigned int a1, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall C_00833834_Serialize(
    C_00833834* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    bool failed = false;

    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archiveAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(&self->field_0008),
            reinterpret_cast<S_00691860*>(archive));
    }

    bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, &self->field_0014, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeFirstCollection(
            reinterpret_cast<int>(&self->field_0020), archiveAddress);
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeSecondCollection(
            reinterpret_cast<unsigned int>(&self->field_0028), archiveAddress);
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, backend, 0)[0x4C / 4])(
                    backend, nullptr, &self->field_0038, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, backend, 0)[0x4C / 4])(
                    backend, nullptr, &self->field_0039, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr,
                    &CSF_AT(uint32_t, self, 0x3C), archiveAddress) == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedState;
    if (CSF_AT(uint8_t, archive, 0) != 0)
    {
        CSF_AT(uint8_t, archive, 0) = 1;
        return 1;
    }

    void* backend = CSF_AT(void*, archive, 0x04);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, backend, 0)[0x7C / 4])(
                backend, nullptr, archiveAddress);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0) = 0;
    return result;
}

HOOK(0x0048D4D0, C_00833834_Serialize, "auto");
