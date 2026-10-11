#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Writes up to requestedLength bytes of sourceData into the sub-stream at this
// object's current position, clamped to the remaining window (field_000c limit).
// Returns the byte count reported by the sink's write slot, or 0 when the window is full.
static int __fastcall C_0085edfc_WriteBufferedChunk(C_0085edfc* self, void* /*edx*/, int sourceData, int requestedLength)
{
    if (self->field_0010 >= self->field_000c)
        return 0;

    auto* owner = reinterpret_cast<uint8_t*>(self->field_0004);
    void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 4));

    uint32_t targetPos = self->field_0008 + self->field_0010;
    int currentPos = ((int(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[4])(target, nullptr);
    if (currentPos != static_cast<int>(targetPos))
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, target, 0)[5])(target, nullptr, 0, static_cast<int>(targetPos));

    uint32_t length = static_cast<uint32_t>(requestedLength);
    if (self->field_0010 + length > self->field_000c)
        length = self->field_000c - self->field_0010;

    int written = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, target, 0)[2])(
        target, nullptr, sourceData, static_cast<int>(length));

    self->field_0010 = self->field_0010 + length;
    return written;
}

HOOK(0x6A29D0, C_0085edfc_WriteBufferedChunk, "auto");
