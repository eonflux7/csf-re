// Reimplementation of 0x0055C6A0 (menu_mp_accounts): clears/refreshes text on child
// controls held at this+0x20..0x2C. The owning class is not identified in the packet,
// so the receiver is typed as void*.
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Virtual slot 0x110 (index 68) on a child control: returns its sub-object (e.g. text/renderer).
static void* MenuAccounts_GetSubobject(void* obj) {
    using GetSubobjectFn = void* (__fastcall*)(void* self, void* edx);
    void** vtbl = *reinterpret_cast<void***>(obj);
    return reinterpret_cast<GetSubobjectFn>(vtbl[68])(obj, nullptr);
}

// Virtual slot 0x3C (index 15) on a sub-object: __thiscall taking one pointer argument.
static int MenuAccounts_SetSubobjectText(void* obj, const void* text) {
    using SetTextFn = int (__fastcall*)(void* self, void* edx, const void* text);
    void** vtbl = *reinterpret_cast<void***>(obj);
    return reinterpret_cast<SetTextFn>(vtbl[15])(obj, nullptr, text);
}

// If the control pointer stored at self+offset is non-null, fetch its sub-object and apply text.
static void MenuAccounts_ApplyTextToChildAt(void* self, uint32_t offset, const void* text) {
    void* control = CSF_AT(void*, self, offset);
    if (control) {
        void* subobject = MenuAccounts_GetSubobject(control);
        MenuAccounts_SetSubobjectText(subobject, text);
    }
}

static int __fastcall MenuAccounts_ClearChildTextsAndRefresh(void* self, void* /*edx*/) {
    // Clear the text of the controls at +0x24, +0x2C, +0x28.
    MenuAccounts_ApplyTextToChildAt(self, 0x24, CSF_STR(0x0082E65C, ""));
    MenuAccounts_ApplyTextToChildAt(self, 0x2C, CSF_STR(0x0082E65C, ""));
    MenuAccounts_ApplyTextToChildAt(self, 0x28, CSF_STR(0x0082E65C, ""));

    // If the flag byte at +0x6E is set, apply the inline buffer at +0x5D to +0x2C and +0x24.
    if (CSF_AT(uint8_t, self, 0x6E) != 0) {
        const void* inlineBuffer = reinterpret_cast<uint8_t*>(self) + 0x5D;
        MenuAccounts_ApplyTextToChildAt(self, 0x2C, inlineBuffer);
        MenuAccounts_ApplyTextToChildAt(self, 0x24, inlineBuffer);
    }

    // Apply the inline buffer at +0x3C to the control at +0x20.
    MenuAccounts_ApplyTextToChildAt(self, 0x20, reinterpret_cast<uint8_t*>(self) + 0x3C);

    // Final step: clear the control at +0x28 and return the raw result of that call
    // (or the null control pointer if there is no control).
    void* lastControl = CSF_AT(void*, self, 0x28);
    if (lastControl) {
        void* subobject = MenuAccounts_GetSubobject(lastControl);
        return MenuAccounts_SetSubobjectText(subobject, CSF_STR(0x0082E65C, ""));
    }
    return 0;
}

HOOK(0x0055C6A0, MenuAccounts_ClearChildTextsAndRefresh, "auto");
