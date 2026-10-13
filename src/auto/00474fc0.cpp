#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082d478_Serialize, 0x00474BD0, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_008310f8_Serialize(
    C_008310f8* self, void* /*edx*/, int archiveAddress)
{
    auto* archive = reinterpret_cast<uint8_t*>(archiveAddress);

    bool beginFailed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        beginFailed =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[0x80 / 4])(
                    stream, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = beginFailed;
    C_0082d478_Serialize(self, nullptr, archiveAddress);

    bool hasError = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!hasError)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[0x38 / 4])(
                    stream, nullptr, &self->field_0034, archive) == -1;
    }

    hasError = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!hasError)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            &self->field_0040, reinterpret_cast<bool*>(archive));
    }

    hasError = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!hasError)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[0x58 / 4])(
                    stream, nullptr, &self->field_0044, archive) == -1;
    }

    hasError = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!hasError)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[0x38 / 4])(
                    stream, nullptr, &self->field_0048, archive) == -1;
    }

    hasError = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!hasError)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[0x54 / 4])(
                    stream, nullptr, &self->field_0054, archive) == -1;
    }

    hasError = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!hasError)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[0x58 / 4])(
                    stream, nullptr, &self->field_0058, archive) == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[0x7C / 4])(
                    stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x00474FC0, C_008310f8_Serialize, "auto");
