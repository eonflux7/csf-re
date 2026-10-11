#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_TextRunCount, 0x008A88C4);
CSF_GLOBAL(uint8_t, g_TextExplicitNewline, 0x008A88CC);
CSF_GLOBAL(uint32_t, g_TextRunEndIndex, 0x008A88D8);

CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void* , int index));
CSF_FUNC(char, __cdecl, Text_ParseControlSequence, 0x00734ED0, (unsigned int* a1, void* a2, unsigned int* a3));
CSF_FUNC(char, __fastcall, C_008670f0_ProcessTextCharacter, 0x007353F0, (void* self, void* edx, wchar_t* String, float* a3, char a4));

static uint16_t C_008670f0_ReadTextCharacter(C_008670f0* self,
                                           uint32_t index)
{
    const uint16_t* text = self->field_0054 < 8
        ? reinterpret_cast<const uint16_t*>(&self->field_0040)
        : reinterpret_cast<const uint16_t*>(self->field_0040);
    return text[index];
}

static char __fastcall C_008670f0_ScanTextRun(
    C_008670f0* self, void* /*edx*/, wchar_t* cursor, float* metrics)
{
    g_TextRunCount = 0;
    g_TextExplicitNewline = 0;

    uint32_t index = CSF_AT(uint32_t, cursor, 0);
    if (index >= self->field_0050)
        return static_cast<char>(index);

    char sawSpace = 0;
    char result;

    for (;;)
    {
        index = CSF_AT(uint32_t, cursor, 0);
        result = static_cast<char>(index);
        if (index >= self->field_0050)
            break;

        if (C_008670f0_ReadTextCharacter(self, index) == 92)
        {
            if (g_TextRunCount)
                break;

            ++index;
            CSF_AT(uint32_t, cursor, 0) = index;
            if (C_008670f0_ReadTextCharacter(self, index) == 110 ||
                C_008670f0_ReadTextCharacter(self, index) == 78)
            {
                result = sawSpace;
                if (sawSpace)
                    g_TextRunCount = 0;
                g_TextExplicitNewline = 1;
                ++CSF_AT(uint32_t, cursor, 0);
                break;
            }

            CSF_AT(uint32_t, cursor, 0) = index - 1;
            result = C_008670f0_ProcessTextCharacter(
                self, nullptr, cursor, metrics, 0);
        }
        else if (C_008670f0_ReadTextCharacter(self, index) == 32)
        {
            if (g_TextRunCount)
            {
                result = sawSpace;
                if (!sawSpace)
                    break;
            }

            result = C_008670f0_ProcessTextCharacter(
                self, nullptr, cursor, metrics, 0);
            sawSpace = 1;
        }
        else
        {
            const bool isControl =
                C_008670f0_ReadTextCharacter(self, index) == 64;
            result = sawSpace;
            if (sawSpace)
                break;

            char control = 0;
            if (isControl &&
                GetIndexedTableValue_6AF8A0(
                    reinterpret_cast<void*>(self->field_0008), nullptr, 0))
            {
                if (self->field_00da)
                {
                    control = Text_ParseControlSequence(
                        reinterpret_cast<unsigned int*>(cursor),
                        reinterpret_cast<uint8_t*>(self) + 0x3C,
                        &self->field_00cc);
                }

                if (control)
                    --CSF_AT(uint32_t, cursor, 0);
                else
                    CSF_AT(uint32_t, cursor, 0) =
                        CSF_AT(uint32_t, cursor, 0);
            }

            result = C_008670f0_ProcessTextCharacter(
                self, nullptr, cursor, metrics, control);
        }

        if (result)
            break;
    }

    g_TextRunEndIndex = CSF_AT(uint32_t, cursor, 0);
    return result;
}

HOOK(0x00735AC0, C_008670f0_ScanTextRun, "auto");
