#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original: 0x005017E0, __fastcall-style callee taking this in ecx.
CSF_FUNC(void*, __fastcall, MenuMessageBox_ResetEmbeddedState, 0x005017E0, (void* self, void* edx));

// Resets five consecutive 28-byte string-like records (flag byte, length, capacity)
// inside the menu buffer object, then resets the embedded message box state.
static int __fastcall MenuBuffer_Init(uint8_t* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x30) = 0;
    CSF_AT(int, self, 0x34) = 15;
    CSF_AT(uint8_t, self, 0x20) = 0;

    CSF_AT(int, self, 0x50) = 15;
    CSF_AT(int, self, 0x4c) = 0;
    CSF_AT(uint8_t, self, 0x3c) = 0;

    CSF_AT(int, self, 0x6c) = 15;
    CSF_AT(int, self, 0x68) = 0;
    CSF_AT(uint8_t, self, 0x58) = 0;

    CSF_AT(int, self, 0x88) = 15;
    CSF_AT(int, self, 0x84) = 0;
    CSF_AT(uint8_t, self, 0x74) = 0;

    CSF_AT(int, self, 0xa4) = 15;
    CSF_AT(int, self, 0xa0) = 0;
    CSF_AT(uint8_t, self, 0x90) = 0;

    CSF_AT(int, self, 0xac) = 0;
    CSF_AT(int, self, 0xb0) = 0;

    MenuMessageBox_ResetEmbeddedState(reinterpret_cast<void*>(self), nullptr);

    return reinterpret_cast<int>(self);
}

HOOK(0x00501870, MenuBuffer_Init, "auto");
