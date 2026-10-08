#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the child registered at the given slot when that child is non-null
// and both the visibility/state byte at +0x7C and the enable byte at +0x8A8 carry the mask.
// Returns the child pointer to dispatch to, or nullptr when the slot is inactive.
static void* MenuMessageBox_FlaggedByteChild(void* self, uint32_t childOffset, uint8_t mask) {
    void* child = CSF_AT(void*, self, childOffset);
    if (child == nullptr) {
        return nullptr;
    }
    if ((CSF_AT(uint8_t, self, 0x7C) & mask) == 0) {
        return nullptr;
    }
    if ((CSF_AT(uint8_t, self, 0x8A8) & mask) == 0) {
        return nullptr;
    }
    return child;
}

// Same as MenuMessageBox_FlaggedByteChild, but the masks are 0x100 / 0x200 / 0x400,
// which the original tests through full dword reads of +0x7C and +0x8A8.
static void* MenuMessageBox_FlaggedDwordChild(void* self, uint32_t childOffset, uint32_t mask) {
    void* child = CSF_AT(void*, self, childOffset);
    if (child == nullptr) {
        return nullptr;
    }
    if ((CSF_AT(uint32_t, self, 0x7C) & mask) == 0) {
        return nullptr;
    }
    if ((CSF_AT(uint32_t, self, 0x8A8) & mask) == 0) {
        return nullptr;
    }
    return child;
}

CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x718660, (void* self, void* edx));

// Dispatches to the first active child in priority order (+0x2C, +0x34, +0x3C, +0x40, +0x30,
// +0x38, +0x44, +0x48, +0x4C, +0x50, +0x54). If none is active, dispatches to the object
// whose address is the dword stored at the pointer held in +0x954.
static int __fastcall CMenuMessageBox_ForwardToFlaggedChild(void* self, void* /*edx*/) {
    if (void* child = MenuMessageBox_FlaggedByteChild(self, 0x2C, 0x01)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedByteChild(self, 0x34, 0x04)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedByteChild(self, 0x3C, 0x20)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedByteChild(self, 0x40, 0x10)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedByteChild(self, 0x30, 0x02)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedByteChild(self, 0x38, 0x08)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedByteChild(self, 0x44, 0x40)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedByteChild(self, 0x48, 0x80)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedDwordChild(self, 0x4C, 0x100)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedDwordChild(self, 0x50, 0x200)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }
    if (void* child = MenuMessageBox_FlaggedDwordChild(self, 0x54, 0x400)) {
        return CGuiSlider_ForwardSelfToParent(child, nullptr);
    }

    // Fallback: the object referenced by the pointer stored at +0x954 is the dispatch target.
    void* fallbackHolder = CSF_AT(void*, self, 0x954);
    void* fallbackTarget = CSF_AT(void*, fallbackHolder, 0);
    return CGuiSlider_ForwardSelfToParent(fallbackTarget, nullptr);
}

HOOK(0x52A680, CMenuMessageBox_ForwardToFlaggedChild, "auto");
