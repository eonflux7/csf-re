#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Campaign_SerializeStringEntries, 0x00440F50,
         (int a1, unsigned int a2));

static int __fastcall C_00831008_SerializeCampaignData(
    C_00831008* self, void* /*edx*/, unsigned int contextAddress)
{
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        // The original tests the whole AL byte, not a canonical C++ bool.
        failed = !((uint8_t(__fastcall*)(void*, void*, uint32_t*, unsigned int))
                       CSF_AT(void**, serializer, 0)[0x80 / 4])(
            serializer, nullptr, nullptr, contextAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Campaign_SerializeStringEntries(
            reinterpret_cast<int>(&self->field_0004), contextAddress);
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Campaign_SerializeStringEntries(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10),
            contextAddress);
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, uint32_t*, unsigned int))
                 CSF_AT(void**, serializer, 0)[0x48 / 4])(
                serializer, nullptr, &self->field_001c, contextAddress);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedContext;

    if (CSF_AT(uint8_t, context, 0x00) != 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, context, 0x04);
    const int result =
        ((int(__fastcall*)(void*, void*, unsigned int))
             CSF_AT(void**, serializer, 0)[0x7C / 4])(
            serializer, nullptr, contextAddress);

    if (!static_cast<uint8_t>(result))
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0x00) = 0;
    return result;
}

HOOK(0x00441850, C_00831008_SerializeCampaignData, "auto");
