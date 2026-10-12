#include <cstdint>
#include "hook.h"

// Copies a three-dword value (e.g. a 3D position) from source into self, returning self.
// The original is __thiscall with one stack argument (ret 4). The dwords are copied as raw
// 32-bit values, so no float conversion takes place.
static uint32_t* __fastcall AiMoveTasks_CopyPosition(uint32_t* self, void* /*edx*/, const uint32_t* source) {
    self[0] = source[0];
    self[1] = source[1];
    self[2] = source[2];
    return self;
}

HOOK(0x4A9ED0, AiMoveTasks_CopyPosition, "auto");
