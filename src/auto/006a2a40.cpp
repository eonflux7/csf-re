#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Moves the output cursor of the inflate window (mode 0 = absolute, 1 = relative
// forward, 2 = backward from the window end), notifies the output sink through its
// vtable slot 0x14 with the new absolute position, and reports whether the cursor
// still lies inside the window. On failure the previous cursor is restored.
static bool __fastcall C_0085edfc_SeekOutputAndCheck(C_0085edfc* self, void* /*edx*/, int mode, int amount)
{
    uint32_t savedCursor = self->field_0010;
    uint32_t amountU = static_cast<uint32_t>(amount);

    // sink = *(uint32_t*)(self->field_0004 + 4)
    void* sinkObj = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004), 4));

    if (mode == 0)
    {
        uint32_t target = amountU + self->field_0008;
        ((void(__fastcall*)(void*, void*, int, uint32_t))CSF_AT(void**, sinkObj, 0)[5])(
            sinkObj, nullptr, 0, target);
        self->field_0010 = amountU;
    }
    else if (mode == 1)
    {
        uint32_t target = amountU + savedCursor + self->field_0008;
        ((void(__fastcall*)(void*, void*, int, uint32_t))CSF_AT(void**, sinkObj, 0)[5])(
            sinkObj, nullptr, 0, target);
        self->field_0010 += amountU;
    }
    else if (mode == 2)
    {
        uint32_t target = self->field_000c + self->field_0008 - amountU;
        ((void(__fastcall*)(void*, void*, int, uint32_t))CSF_AT(void**, sinkObj, 0)[5])(
            sinkObj, nullptr, 0, target);
        self->field_0010 = self->field_000c - amountU;
    }

    if (self->field_0010 <= self->field_000c)
        return true;

    self->field_0010 = savedCursor;
    return false;
}

HOOK(0x006A2A40, C_0085edfc_SeekOutputAndCheck, "auto");
