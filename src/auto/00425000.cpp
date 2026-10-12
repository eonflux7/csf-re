#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwarder bound by address. Called with ecx still holding `this`, no stack args.
CSF_FUNC(uint32_t, __fastcall, AiTaskStates_ForwardToSceneChildSlot7, 0x0041E340, (void* self, void* edx));

// Unnamed callee at 0x6AEF20, bound as specified in the packet.
CSF_FUNC(int, __fastcall, sub_6AEF20, 0x006AEF20, (void* self, void* edx));

// Lazily creates the slot-7 child object stored at this+0x44, then notifies it at +4.
static int __fastcall C_0082fcd8_EnsureSlot7ChildAndNotify(C_0082fcd8* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x44) == 0)
    {
        CSF_AT(uint32_t, self, 0x44) = AiTaskStates_ForwardToSceneChildSlot7(self, nullptr);
    }

    uint8_t* childBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x44));
    sub_6AEF20(childBase + 4, nullptr);

    return static_cast<int>(CSF_AT(uint32_t, self, 0x44));
}

HOOK(0x425000, C_0082fcd8_EnsureSlot7ChildAndNotify, "auto");
