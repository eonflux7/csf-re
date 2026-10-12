#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0,
         (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));

// Inserts one wide character at the position given by the current length
// (field_0014) within the string buffer. The buffer is stored inline at +4 while
// its capacity (field_0018) is below 8; otherwise +4 holds a heap pointer.
static uint32_t* __fastcall SaveSlots_ApplyIndex(S_004f21d0* self, void* /*edx*/, int16_t a2)
{
    const uint32_t capacity = self->field_0018;
    const bool usesHeapBuffer = capacity >= 8;

    // Base of the character storage: inline at +4, or the heap pointer stored at +4.
    uint32_t bufferStart = reinterpret_cast<uint32_t>(&self->field_0004);
    if (usesHeapBuffer)
        bufferStart = reinterpret_cast<uint32_t>(*reinterpret_cast<uint16_t**>(&self->field_0004));

    // Address one past the last character (length is in wide characters).
    uint32_t cursorAddr = bufferStart;
    if (usesHeapBuffer)
        cursorAddr = reinterpret_cast<uint32_t>(*reinterpret_cast<uint16_t**>(&self->field_0004));
    cursorAddr += self->field_0014 * 2;

    // Convert the byte distance to a character index, but only for a non-null cursor.
    uint32_t insertIndex = cursorAddr;
    if (insertIndex != 0)
        insertIndex = static_cast<uint32_t>((static_cast<int32_t>(cursorAddr - bufferStart)) >> 1);

    InsertWideCharacters(self, nullptr, insertIndex, 1u, a2);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00697020, SaveSlots_ApplyIndex, "auto");
