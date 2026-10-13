#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// A scene channel slot holds a pointer to a block whose first dword is the
// channel's data count; the slot counts only when the pointer is set and that dword is nonzero.
static bool SceneChannelSlotHasData(uint32_t slotPointerValue) {
    uint32_t* channelData = reinterpret_cast<uint32_t*>(slotPointerValue);
    return channelData != nullptr && *channelData != 0;
}

// int __thiscall HAnim_GetSceneChannelCount(S_006c3320 *this)
static int __fastcall HAnim_GetSceneChannelCount(S_006c3320* self, void* /*edx*/) {
    int result = 0;

    if (SceneChannelSlotHasData(CSF_AT(uint32_t, self, 0x04)))
        result = 1;
    if (SceneChannelSlotHasData(CSF_AT(uint32_t, self, 0x08)))
        ++result;
    if (SceneChannelSlotHasData(CSF_AT(uint32_t, self, 0x0C)))
        ++result;
    if (SceneChannelSlotHasData(CSF_AT(uint32_t, self, 0x10)))
        ++result;
    if (SceneChannelSlotHasData(CSF_AT(uint32_t, self, 0x14)))
        ++result;
    if (SceneChannelSlotHasData(CSF_AT(uint32_t, self, 0x18)))
        ++result;
    if (SceneChannelSlotHasData(CSF_AT(uint32_t, self, 0x1C)))
        ++result;
    if (SceneChannelSlotHasData(CSF_AT(uint32_t, self, 0x20)))
        ++result;

    return result;
}

HOOK(0x006C2080, HAnim_GetSceneChannelCount, "auto");
