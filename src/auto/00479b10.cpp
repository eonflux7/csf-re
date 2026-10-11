#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_008319c0_SerializeObjectState(
    C_008319c0* self, void* /*edx*/, void* archive)
{
    bool skipFields = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        skipFields = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = skipFields;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!skipFields)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, &self->field_000c, archive) == -1;
    }

    const bool serializeReference = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (serializeReference)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            &self->field_0004, static_cast<bool*>(archive));
    }

    const bool finalize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (finalize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
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

HOOK(0x00479B10, C_008319c0_SerializeObjectState, "auto");
