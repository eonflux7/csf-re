#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (void* self, void* edx, int16_t a2));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static S_004f21d0* __fastcall C_008670f0_CopyTextThroughEntries(
    C_008670f0* self, void* /*edx*/, S_004f21d0* output, char copyEntireText)
{
    TextInputSubobject_Init(output, nullptr, &g_byte_82E65C);

    if (copyEntireText)
    {
        AssignStringRange_488830(
            output, nullptr, reinterpret_cast<uint8_t*>(self) + 0x3C,
            0, 0xFFFFFFFF);
        return output;
    }

    const uint32_t entryCount = self->field_0080;
    const uint32_t textLength = self->field_0050;
    uint32_t textIndex = 0;
    uint32_t entryIndex = 0;
    uint32_t entryOffset = 0;

    while (entryIndex < entryCount)
    {
        if (textIndex >= textLength)
            break;

        for (;;)
        {
            const uint8_t* textData = self->field_0054 < 8
                ? reinterpret_cast<const uint8_t*>(&self->field_0040)
                : reinterpret_cast<const uint8_t*>(self->field_0040);

            const uint8_t* entryText = CSF_AT(
                uint8_t*, reinterpret_cast<uint8_t*>(self->field_0078),
                entryOffset);
            const uint16_t entryCharacter = CSF_AT(uint16_t, entryText, 0);
            const bool matched =
                entryCharacter == CSF_AT(uint16_t, textData, 2 * textIndex);

            const uint32_t capacity = self->field_0054;
            const uint8_t* appendData = capacity < 8
                ? reinterpret_cast<const uint8_t*>(&self->field_0040)
                : reinterpret_cast<const uint8_t*>(self->field_0040);

            SaveSlots_ApplyIndex(
                output, nullptr,
                static_cast<int16_t>(CSF_AT(uint16_t, appendData, 2 * textIndex)));
            ++textIndex;

            if (matched)
                break;
        }

        ++entryIndex;
        entryOffset += 28;
    }

    return output;
}

HOOK(0x007359C0, C_008670f0_CopyTextThroughEntries, "auto");
