#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082e520_Serialize, 0x0041E140,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10,
         (void* a3, bool* a2));

static int __fastcall C_0082deb8_Serialize(
    C_0082deb8* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archiveAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    C_0082e520_Serialize(self, nullptr, archiveAddress);

    bool canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            &self->field_0010, reinterpret_cast<bool*>(archive));
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_0014, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, &self->field_0018, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, &self->field_0024, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, &self->field_003d, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, &self->field_003e, archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, &CSF_AT(uint32_t, self, 0x40),
                    archiveAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        const bool completed =
            reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, archiveAddress);
        if (completed)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return completed;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x00410860, C_0082deb8_Serialize, "auto");
