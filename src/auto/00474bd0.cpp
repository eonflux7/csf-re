#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall C_0082d478_Serialize(
    C_0082d478* self, void* /*edx*/, int archiveAddress)
{
    auto* archive = reinterpret_cast<S_00691860*>(archiveAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_0004, archive) == -1;
    }

    bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(&self->field_0008), archive);
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(&self->field_0014), archive);
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_0024, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, &self->field_002c, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_0020, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, &self->field_0030, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, &CSF_AT(uint32_t, self, 0x28), archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        // The original tests AL but forwards the complete EAX on success.
        const int result =
            ((int(__fastcall*)(void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x00474BD0, C_0082d478_Serialize, "auto");
