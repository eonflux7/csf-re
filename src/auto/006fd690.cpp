#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall GfxClumpLoader_SerializeFields(void* self, void* /*edx*/, void* state)
{
    auto* fields = static_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
                       CSF_AT(void**, stream, 0)[0x80 / 4])(
            stream, nullptr, 0, state);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0) = failed;
    CSF_AT(uint32_t, state, 0x1C) = 0;

    if (!failed)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, stream, 0)[0x44 / 4])(
                stream, nullptr, fields + 0x39, state) == -1;
    }

    bool canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, stream, 0)[0x0C / 4])(
                stream, nullptr, fields + 0x3C, state) == -1;
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, stream, 0)[0x0C / 4])(
                stream, nullptr, fields + 0x6C, state) == -1;
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, stream, 0)[0x0C / 4])(
                stream, nullptr, fields + 0x4C, state) == -1;
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, stream, 0)[0x0C / 4])(
                stream, nullptr, fields + 0x5C, state) == -1;
    }

    canContinue = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, stream, 0)[0x44 / 4])(
                stream, nullptr, fields + 0x7C, state) == -1;
    }

    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        const uint8_t result =
            ((uint8_t(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, state);
        if (result != 0)
        {
            CSF_AT(uint8_t, state, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x006FD690, GfxClumpLoader_SerializeFields, "auto");
