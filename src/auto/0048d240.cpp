#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall S_00691860_SerializeObjectState(
    S_00691860* self, void* /*edx*/, bool* archive)
{
    const uint32_t savedState = CSF_AT(uint32_t, archive, 0x1C);

    bool skipSerialization = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        skipSerialization =
            !((bool(__fastcall*)(void*, void*, int, bool*))
                CSF_AT(void**, stream, 0)[0x80 / 4])(
                    stream, nullptr, 0, archive);
    }

    CSF_AT(uint8_t, archive, 0) = skipSerialization;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!skipSerialization)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(self, archive);
    }

    bool canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, void*, bool*))
                CSF_AT(void**, stream, 0)[0x4C / 4])(
                    stream, nullptr, &self->field_0004, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, void*, bool*))
                CSF_AT(void**, stream, 0)[0x4C / 4])(
                    stream, nullptr,
                    reinterpret_cast<uint8_t*>(&self->field_0004) + 1,
                    archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedState;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, bool*))
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

HOOK(0x0048D240, S_00691860_SerializeObjectState, "auto");
