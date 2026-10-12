#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game heap free (__cdecl, pops nothing; caller cleans up with add esp, 4).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the four owned buffers of C_00863018 (and clears their size/cursor
// fields) when the "buffers loaded" flag at +0x21 is set, then clears the flag.
// Returns the low byte of the last pointer-field value, as the original does.
static char __fastcall C_00863018_ReleaseBuffers(C_00863018* self, void* /*edx*/) {
    char result = 0;

    if (self->field_0021 != 0) {
        // Buffer 1: +0x24, counters at +0x28 / +0x2c
        uint32_t firstBlock = self->field_0024;
        self->field_0028 = 0;
        self->field_002c = 0;
        if (firstBlock) {
            Mem_Free(reinterpret_cast<void*>(firstBlock));
            self->field_0024 = 0;
        }

        // Buffer 2: +0x34, counters at +0x38 / +0x3c
        uint32_t secondBlock = self->field_0034;
        self->field_0038 = 0;
        self->field_003c = 0;
        if (secondBlock) {
            Mem_Free(reinterpret_cast<void*>(secondBlock));
            self->field_0034 = 0;
        }

        // Buffer 3: +0x44, counters at +0x48 / +0x4c
        uint32_t thirdBlock = self->field_0044;
        self->field_0048 = 0;
        self->field_004c = 0;
        if (thirdBlock) {
            Mem_Free(reinterpret_cast<void*>(thirdBlock));
            self->field_0044 = 0;
        }

        // Buffer 4: +0x54, counters at +0x58 / +0x5c
        uint32_t fourthBlock = self->field_0054;
        self->field_0058 = 0;
        self->field_005c = 0;
        result = static_cast<char>(fourthBlock);
        if (fourthBlock) {
            result = static_cast<char>(Mem_Free(reinterpret_cast<void*>(fourthBlock)));
            self->field_0054 = 0;
        }

        self->field_0021 = 0;
    }

    return result;
}

HOOK(0x6FE470, C_00863018_ReleaseBuffers, "auto");
