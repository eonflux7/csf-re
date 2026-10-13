#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall S_0041f0a0_SerializeBaseFields(
    S_0041f0a0* self, void* /*edx*/, bool* archive)
{
    bool skipFields = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 4);
        skipFields =
            !((bool (__fastcall*)(void*, void*, int, bool*))
                  CSF_AT(void**, stream, 0)[0x80 / 4])(
                      stream, nullptr, 0, archive);
    }

    const int savedContext = CSF_AT(int, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = skipFields;
    CSF_AT(int, archive, 0x1C) = 0;
    if (!skipFields)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            reinterpret_cast<uint8_t*>(self) + 4, archive);
    }

    bool canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            reinterpret_cast<uint8_t*>(self) + 8, archive);
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            reinterpret_cast<uint8_t*>(self) + 0xC, archive);
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(int, archive, 0x1C) = savedContext;
    if (canContinue)
    {
        void* stream = CSF_AT(void*, archive, 4);
        const int result =
            ((int (__fastcall*)(void*, void*, bool*))
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

HOOK(0x004DB4F0, S_0041f0a0_SerializeBaseFields, "auto");
