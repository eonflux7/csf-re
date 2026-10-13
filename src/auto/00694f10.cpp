#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00694cd0_EnsureCapacity, 0x00694CD0,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_00694cd0_WriteIndentation, 0x00694D50,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, S_00694cd0_AppendNarrowString, 0x00694DC0,
         (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, S_00694cd0_AppendWideString, 0x00694E10,
         (void* self, void* edx, void* a2));

CSF_GLOBAL(uint16_t, g_CoreStringFormatLineEnding, 0x0085E680);
static constexpr uint16_t k_CoreStringFormatCRLF = 2573;

static uint16_t* __fastcall S_00694cd0_WriteWideString(
    S_00694cd0* self, void* /*edx*/, const void* text, const char* prefix)
{
    (void)CSF_VTBL(0x00694CD0);

    if (CSF_AT(uint8_t, self, 0x11C + 8 * self->field_0004))
    {
        S_00694cd0_AppendNarrowString(
            self, nullptr, const_cast<char*>(CSF_STR(0x0083F284, " ")));
    }
    else
    {
        S_00694cd0_AppendNarrowString(
            self, nullptr, reinterpret_cast<char*>(self) + 0x14);
    }

    if (prefix)
    {
        S_00694cd0_AppendNarrowString(self, nullptr, const_cast<char*>(prefix));
        S_00694cd0_AppendNarrowString(
            self, nullptr, const_cast<char*>(CSF_STR(0x0083F284, " ")));
        S_00694cd0_WriteIndentation(
            self, nullptr, static_cast<int>(self->field_0004));
    }

    if (CSF_AT(uint8_t, self, 0x11C + 8 * self->field_0004))
    {
        return reinterpret_cast<uint16_t*>(
            S_00694cd0_AppendWideString(self, nullptr, const_cast<void*>(text)));
    }

    S_00694cd0_AppendWideString(self, nullptr, const_cast<void*>(text));
    S_00694cd0_EnsureCapacity(self, nullptr, 2);

    uint16_t* result = reinterpret_cast<uint16_t*>(
        self->field_0000 + self->field_000c);

    // The original's constant word is 2573 (0xA0D). Preserve its addressed read.
    const uint16_t lineEnding = g_CoreStringFormatLineEnding;
    *result = lineEnding == k_CoreStringFormatCRLF
        ? k_CoreStringFormatCRLF
        : lineEnding;
    self->field_000c += 2;
    return result;
}

HOOK(0x00694F10, S_00694cd0_WriteWideString, "auto");
