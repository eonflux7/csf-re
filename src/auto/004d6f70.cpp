#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original vtable slot 0x80 (offset 128 / 4 = 32), returns a state code.
static constexpr int kChildStateSlot = 0x80 / 4;
static constexpr int kChildStateFourteen = 14;

static int __fastcall C_0083dbb0_HasChildInStateFourteen(C_0083dbb0* self, void* /*edx*/)
{
    uint32_t childPtr = CSF_AT(uint32_t, self, 0x10);
    if (childPtr == 0) {
        return 0;
    }

    void* child = reinterpret_cast<void*>(childPtr);
    int childState = ((int(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[kChildStateSlot])(child, nullptr);

    return childState == kChildStateFourteen ? 1 : 0;
}

HOOK(0x004D6F70, C_0083dbb0_HasChildInStateFourteen, "auto");
