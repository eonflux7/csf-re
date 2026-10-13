#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_00862490::CopyTransformFromMessage (0x6F1BE0)
// Copies three floats from the message at msg+0x20 into this+0x2C..0x34,
// then dispatches the virtual slot 0x38 with the message at msg+0x30.
static int __fastcall C_00862490_CopyTransformFromMessage(C_00862490* self, void* /*edx*/, uint32_t* a2)
{
    uint8_t* msg = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*a2));

    self->field_002c = *reinterpret_cast<float*>(msg + 0x20);
    CSF_AT(float, self, 0x30) = *reinterpret_cast<float*>(msg + 0x24);
    CSF_AT(float, self, 0x34) = *reinterpret_cast<float*>(msg + 0x28);

    using SlotFn = int(__fastcall*)(void* self, void* edx, int arg);
    SlotFn slot38 = reinterpret_cast<SlotFn>(CSF_AT(void**, self, 0)[0x38 / 4]);
    return slot38(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(msg + 0x30)));
}

HOOK(0x006F1BE0, C_00862490_CopyTransformFromMessage, "auto");
