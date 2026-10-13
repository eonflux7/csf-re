#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original's 2-byte line terminator word: 0x0A0D (bytes 0x0D 0x0A, i.e. "\r\n" in little-endian).
// The original reads it from 0x85E680, which holds this value.
static constexpr uint16_t kCrlfTerminatorWord = 0x0A0D;

CSF_GLOBAL(uint16_t, g_CrlfTerminatorWord, 0x0085E680);

CSF_FUNC(unsigned int, __fastcall, StrBuf_ReserveBytes, 0x00694CD0, (void* self, void* edx, int a2));

// __thiscall(S_00694cd0* this, const char* text) -> ret 4
// Appends `text` to the buffer, then appends a CRLF word terminator. Returns the new write offset.
static int __fastcall StrBuf_AppendLineWithCrlf(S_00694cd0* self, void* /*edx*/, const char* text)
{
    size_t textLength = std::strlen(text);

    StrBuf_ReserveBytes(self, nullptr, static_cast<int>(textLength));

    uint8_t* destination = reinterpret_cast<uint8_t*>(self->field_0000) + self->field_000c;
    std::memcpy(destination, text, textLength);
    self->field_000c += static_cast<uint32_t>(textLength);

    StrBuf_ReserveBytes(self, nullptr, 2);

    uint8_t* terminatorDest = reinterpret_cast<uint8_t*>(self->field_0000) + self->field_000c;
    *reinterpret_cast<uint16_t*>(terminatorDest) = kCrlfTerminatorWord;

    int newOffset = static_cast<int>(self->field_000c) + 2;
    self->field_000c = static_cast<uint32_t>(newOffset);
    return newOffset;
}

HOOK(0x00694EA0, StrBuf_AppendLineWithCrlf, "auto");
