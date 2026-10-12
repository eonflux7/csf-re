#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082e1e8_Serialize, 0x0041B360, (C_0082e1e8* self, void* , int contextAddress));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_0082db60_Serialize(
    C_0082db60* self, void* /*edx*/, int contextAddress)
{
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);

    bool initialError = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        initialError =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, archive, 0)[0x80 / 4])(
                    archive, nullptr, 0, contextAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = initialError;
    C_0082e1e8_Serialize(self, nullptr, contextAddress);

    bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 1;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeObjectRef(
            &self->field_008c, reinterpret_cast<bool*>(context));
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, archive, 0)[0x58 / 4])(
                    archive, nullptr, &self->field_0090, contextAddress);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, archive, 0)[0x38 / 4])(
                    archive, nullptr, &self->field_0094, contextAddress);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr, &self->field_00a0, contextAddress);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr,
                    &CSF_AT(uint32_t, self, 0xA4), contextAddress);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (ready)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        const bool result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, archive, 0)[0x7C / 4])(
                    archive, nullptr, contextAddress);
        if (result)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0040CF70, C_0082db60_Serialize, "auto");
