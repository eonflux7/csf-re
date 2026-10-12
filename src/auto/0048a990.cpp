#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, Archive_SerializeEntries, 0x0048A7B0,
         (int a1, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10,
         (void* a3, bool* a2));

static int __fastcall Archive_SerializeObjectAndEntries(
    void* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, serializer, 0x00)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeObjectRef(
            static_cast<uint8_t*>(self) + 4,
            static_cast<bool*>(context));
    }

    const bool serializeEntries = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeEntries)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeEntries(
            reinterpret_cast<int>(static_cast<uint8_t*>(self) + 8),
            reinterpret_cast<int>(context));
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, serializer, 0x00)[0x7C / 4])(
                    serializer, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0048A990, Archive_SerializeObjectAndEntries, "auto");
