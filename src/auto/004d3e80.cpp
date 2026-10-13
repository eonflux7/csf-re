#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, S_0041f0a0_SerializeBaseState, 0x004D19C0, (S_0041f0a0* self, void* , S_0041f0a0* archive));

static int __fastcall S_0041f0a0_SerializeState(
    S_0041f0a0* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(static_cast<uintptr_t>(archiveAddress));

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    S_0041f0a0_SerializeBaseState(
        self, nullptr, reinterpret_cast<S_0041f0a0*>(archive));

    bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &self->field_0040, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &CSF_AT(uint8_t, self, 0x44), archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &CSF_AT(uint8_t, self, 0x3C), archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &CSF_AT(uint8_t, self, 0x48), archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &CSF_AT(uint8_t, self, 0x4C), archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, &CSF_AT(uint8_t, self, 0x54), archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            static_cast<int>(reinterpret_cast<uintptr_t>(&CSF_AT(uint8_t, self, 0x60))),
            reinterpret_cast<S_00691860*>(archive));
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0)
        self->field_0040 |= 0x100u;

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004D3E80, S_0041f0a0_SerializeState, "auto");
