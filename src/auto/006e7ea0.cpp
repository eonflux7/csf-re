#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwFrameRemoveChild, 0x00623750, (int a1));

// Removes the frame from its parent unless it is already present in the
// holder's pointer list. Returns the frame.
static int __cdecl GfxBones_RemoveFrameIfNotInList(int frame, uint32_t** holderSlot)
{
    uint32_t* holder = *holderSlot;
    uint32_t count = CSF_AT(uint32_t, holder, 0x08);
    uint32_t* items = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, holder, 0x00));

    if (count == 0) {
        RwFrameRemoveChild(frame);
        return frame;
    }

    uint32_t index = 0;
    while (items[index] != static_cast<uint32_t>(frame)) {
        ++index;
        if (index >= count) {
            RwFrameRemoveChild(frame);
            return frame;
        }
    }
    // Found in the list: nothing to remove.
    return frame;
}

HOOK(0x6E7EA0, GfxBones_RemoveFrameIfNotInList, "auto");
