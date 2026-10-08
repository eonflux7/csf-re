#include <cstdint>
#include "globals.h"
#include "hook.h"

// Thumbnail entries are 0x44 bytes: +0x20 primary sub-object, +0x24 optional secondary sub-object.
static constexpr uint32_t kThumbnailEntryStride = 0x44;

CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x718660, (void* self, void* edx));

// Virtual slot +0xA8 (__thiscall, one stack argument, returns eax).
static int MenuExtras_CallVirtualSetState(void* obj, int state) {
    using SetStateFn = int(__fastcall*)(void*, void*, int);
    SetStateFn fn = (SetStateFn)CSF_AT(void**, obj, 0)[168 / 4];
    return fn(obj, nullptr, state);
}

// Virtual slot +0xFC (__thiscall, one stack argument: name string, returns child pointer).
static void* MenuExtras_FindChildByName(void* owner, const char* name) {
    using FindChildFn = void*(__fastcall*)(void*, void*, const char*);
    FindChildFn fn = (FindChildFn)CSF_AT(void**, owner, 0)[252 / 4];
    return fn(owner, nullptr, name);
}

// Address of the thumbnail entry at byteOffset from the array begin pointer passed in.
static uint8_t* MenuExtras_ThumbnailEntryAt(uint32_t arrayBegin, uint32_t byteOffset) {
    return reinterpret_cast<uint8_t*>(arrayBegin + byteOffset);
}

static int __fastcall CMenuExtras_ApplyPanelMode(void* self, void* /*edx*/, char flag) {
    // Original reads [self+0x38] once and reuses it for the null test and the subtraction.
    const uint32_t arrayBegin = CSF_AT(uint32_t, self, 0x38);
    int thumbCount = 0;
    if (arrayBegin != 0) {
        const uint32_t arrayEnd = CSF_AT(uint32_t, self, 0x3C);
        thumbCount = static_cast<int32_t>(arrayEnd - arrayBegin) / 68;
    }

    // Original: sete bl / sete cl -> 1 when flag == 0, else 0.
    const int childState = (flag == 0) ? 1 : 0;

    if (thumbCount != 0) {
        int remaining = thumbCount;
        uint32_t entryOffset = 0;
        do {
            // [self+0x38] is re-read on each iteration, as the original does.
            void* primary = CSF_AT(void*,
                MenuExtras_ThumbnailEntryAt(CSF_AT(uint32_t, self, 0x38), entryOffset), 0x20);
            MenuExtras_CallVirtualSetState(primary, childState);

            void* secondary = CSF_AT(void*,
                MenuExtras_ThumbnailEntryAt(CSF_AT(uint32_t, self, 0x38), entryOffset), 0x24);
            if (secondary != nullptr) {
                MenuExtras_CallVirtualSetState(secondary, childState);
            }
            entryOffset += kThumbnailEntryStride;
        } while (--remaining != 0);
    }

    void* makingOfPanel = MenuExtras_FindChildByName(CSF_AT(void*, self, 0x18), CSF_STR(0x843C58, "MakingOf"));
    if (makingOfPanel != nullptr) {
        MenuExtras_CallVirtualSetState(makingOfPanel, childState);
    }

    void* acceptButton = MenuExtras_FindChildByName(CSF_AT(void*, self, 0x18), CSF_STR(0x843C50, "Accept"));
    int result = static_cast<int>(reinterpret_cast<intptr_t>(acceptButton));
    if (result != 0) {
        result = MenuExtras_CallVirtualSetState(acceptButton, childState);
    }

    if (flag != 0) {
        const uint32_t selectedIndex = CSF_AT(uint32_t, self, 0x28);
        void* selectedPanel = CSF_AT(void*,
            MenuExtras_ThumbnailEntryAt(CSF_AT(uint32_t, self, 0x38), selectedIndex * kThumbnailEntryStride), 0x20);
        return CGuiSlider_ForwardSelfToParent(selectedPanel, nullptr);
    }
    return result;
}

HOOK(0x546590, CMenuExtras_ApplyPanelMode, "auto");
