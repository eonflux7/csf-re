#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reserves `count` bytes at the end of the buffer (advances field_000c).
CSF_FUNC(unsigned int, __fastcall, S_00694cd0_ReserveBytes, 0x00694CD0, (void* self, void* edx, int a2));
// Returns the length in UTF-16 code units of a zero-terminated wide string.
CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));
// Game data byte at 0x83AB18 (the original reads it; the quote value it holds is 34).
CSF_GLOBAL(uint8_t, g_QuoteChar, 0x0083AB18);

// Double-quote character (ASCII 34) written before and after the text.
static constexpr uint8_t kQuoteCharacter = 34;

// Appends a quoted copy of a wide string to the S_00694cd0 buffer and returns the new write offset.
static int __fastcall S_00694cd0_AppendQuotedWideText(S_00694cd0* self, void* /*edx*/, const void* wideText)
{
    S_00694cd0_ReserveBytes(self, nullptr, 1);
    {
        uint8_t* buffer = reinterpret_cast<uint8_t*>(self->field_0000);
        buffer[self->field_000c] = kQuoteCharacter;
    }
    ++self->field_000c;

    int byteCount = 2 * GetWideTextLength(static_cast<std::uint16_t*>(const_cast<void*>(wideText)));
    S_00694cd0_ReserveBytes(self, nullptr, byteCount);

    uint8_t* destination = reinterpret_cast<uint8_t*>(self->field_0000) + self->field_000c;
    std::memcpy(destination, wideText, static_cast<std::size_t>(byteCount));
    self->field_000c += static_cast<uint32_t>(byteCount);

    S_00694cd0_ReserveBytes(self, nullptr, 1);
    {
        uint8_t* buffer = reinterpret_cast<uint8_t*>(self->field_0000);
        buffer[self->field_000c] = kQuoteCharacter;
    }
    ++self->field_000c;
    return static_cast<int>(self->field_000c);
}

HOOK(0x00694E10, S_00694cd0_AppendQuotedWideText, "auto");
