#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_ComponentPools, 0x008BAF48);

CSF_FUNC(int, __fastcall, CoreParser_ReadIndexedEntry, 0x00692B50, (void* self, void* , int index));
CSF_FUNC(uint8_t*, __fastcall, CoreZlibLog_ReadFallbackWideAttributeText, 0x00693460, (void* self, void* edx, int64_t a2));
CSF_FUNC(uint8_t*, __cdecl, CoreZlibLog_ConvertAttributeTextToWide, 0x006955E0, (uint16_t* a1, uint16_t* a2, int a3));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static uint8_t* __fastcall S_00692b80_ReadWideAttributeText(
    S_00692b80* self, void* /*edx*/, int64_t outputArguments)
{
    S_00692b80* attribute = self;

    if (attribute->field_000b == 0xFF)
    {
        return CoreZlibLog_ReadFallbackWideAttributeText(
            reinterpret_cast<void*>(attribute->field_0000),
            nullptr, outputArguments);
    }

    while (!CSF_AT(uint8_t, attribute, 0x0A))
    {
        if (attribute->field_000b == 0xFF)
        {
            attribute = CSF_AT(
                S_00692b80*,
                reinterpret_cast<void*>(attribute->field_0000), 0x18);
        }
        else
        {
            void* pool = CSF_AT(
                void*, &g_ComponentPools, attribute->field_000b * 4);
            const uint32_t base = CSF_AT(uint32_t, pool, 0);
            const uint32_t nextIndex =
                (reinterpret_cast<uint32_t>(attribute) - base) / 0xC + 1;

            attribute = reinterpret_cast<S_00692b80*>(base + 12 * nextIndex);
            if (!CSF_AT(uint8_t, pool, 0x14))
                attribute->field_000b = CSF_AT(uint8_t, pool, 0x15);
        }

        if (attribute->field_000b == 0xFF)
        {
            return CoreZlibLog_ReadFallbackWideAttributeText(
                reinterpret_cast<void*>(attribute->field_0000),
                nullptr, outputArguments);
        }
    }

    alignas(8) char buffer[0x400] = {};

    switch (CSF_AT(uint8_t, attribute, 0x0A))
    {
    case 3:
        _sprintf(buffer, CSF_STR(0x0083090C, "%d"),
                 CSF_AT(int, attribute, 0x04));
        break;

    case 4:
        _sprintf(buffer, CSF_STR(0x00840C3C, "%f"),
                 static_cast<double>(CSF_AT(float, attribute, 0x04)));
        break;

    case 5:
    {
        const int index = CSF_AT(int, attribute, 0x04);
        void* pool = CSF_AT(
            void*, &g_ComponentPools, attribute->field_000b * 4);
        const char* source = reinterpret_cast<const char*>(
            CoreParser_ReadIndexedEntry(pool, nullptr, index));
        char* destination = buffer;
        char character;
        do
        {
            character = *source++;
            *destination++ = character;
        } while (character);
        break;
    }
    }

    return CoreZlibLog_ConvertAttributeTextToWide(
        reinterpret_cast<uint16_t*>(buffer),
        reinterpret_cast<uint16_t*>(static_cast<uint32_t>(outputArguments)),
        static_cast<int>(outputArguments >> 32));
}

HOOK(0x00692E40, S_00692b80_ReadWideAttributeText, "auto");
