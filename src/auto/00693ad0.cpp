#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: game allocator wrapper, cdecl, one argument (byte count), returns block or null.
CSF_FUNC(void*, __cdecl, AllocateCursorBlock, 0x6934E0, (uint32_t a1));

// __thiscall(this in ecx, a2 on stack), ret 4.
// When a2 is nonzero, allocates a 0x4C-byte cursor block and initializes it;
// otherwise clears the pointer. Either way returns this.
static uint8_t* __fastcall S_00693ad0_Init(S_00693ad0* self, void* /*edx*/, char enableCursorBlock)
{
    if (enableCursorBlock)
    {
        void* cursorBlock = AllocateCursorBlock(0x4c);
        if (cursorBlock)
        {
            CSF_AT(uint32_t, cursorBlock, 0x08) = 0;
            CSF_AT(uint32_t, cursorBlock, 0x0C) = 0;
            CSF_AT(uint32_t, cursorBlock, 0x10) = 0;
            CSF_AT(uint32_t, cursorBlock, 0x14) = 10;
            CSF_AT(uint32_t, cursorBlock, 0x04) = 0;
            CSF_AT(uint32_t, cursorBlock, 0x40) = 0;
            CSF_AT(uint32_t, cursorBlock, 0x00) = 0xFFFFFFFFu;
            CSF_AT(uint32_t, cursorBlock, 0x14) = 100;
            self->field_0000 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(cursorBlock));
        }
        else
        {
            self->field_0000 = 0;
        }
        self->field_000b = 0xFF;
        return reinterpret_cast<uint8_t*>(self);
    }

    self->field_0000 = 0;
    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x693AD0, S_00693ad0_Init, "auto");
