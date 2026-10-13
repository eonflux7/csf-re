#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (S_004f21d0* self, void* , int16_t a2));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(void*, __fastcall, AppendNarrowToWideString, 0x00697150, (void* self, void* , uint8_t* a2));
CSF_FUNC(char, __fastcall, S_00698c10_PrepareTokenRead, 0x00699020, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(uint8_t, g_QuotedTextSuffix, 0x0085E71C);

static bool __fastcall S_00698c10_ParseQuotedText(
    S_00698c10* self, void* /*edx*/, S_004f21d0* output)
{
    bool opened = false;
    if (!S_00698c10_PrepareTokenRead(self, nullptr))
    {
        const uint32_t current = self->field_0018;
        const uint32_t next = current + 2;
        if (next <= self->field_001c)
        {
            const int16_t character =
                *reinterpret_cast<const int16_t*>(current);
            self->field_0018 = next;
            opened = character == 34;
        }
    }

    TextInputSubobject_Init(output, nullptr, &g_byte_82E65C);
    if (!opened)
        return false;

    bool closed = false;
    for (;;)
    {
        const uint32_t current = self->field_0018;
        const uint32_t next = current + 2;
        if (next > self->field_001c)
            break;

        const int16_t character =
            *reinterpret_cast<const int16_t*>(current);
        self->field_0018 = next;

        if (character != 34)
        {
            SaveSlots_ApplyIndex(output, nullptr, character);
            continue;
        }

        if (*reinterpret_cast<const uint16_t*>(next - 4) != 92)
        {
            closed = true;
            break;
        }

        uint16_t* inlineText = &output->field_0004;
        uint16_t* text = inlineText;
        if (output->field_0018 >= 8)
            text = *reinterpret_cast<uint16_t**>(inlineText);

        const int length = GetWideTextLength(text);

        if (output->field_0018 >= 8)
            inlineText = *reinterpret_cast<uint16_t**>(inlineText);
        inlineText[length - 1] = 34;
    }

    AppendNarrowToWideString(output, nullptr, &g_QuotedTextSuffix);
    return closed;
}

HOOK(0x006990B0, S_00698c10_ParseQuotedText, "auto");
