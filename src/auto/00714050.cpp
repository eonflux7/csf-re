#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Virtual slot 0x280 (byte offset 640) on the texture object: called with the
// listbox's byte flag at +0x1f4 (zero-extended) as its single argument.
// Virtual slot 0x108 (byte offset 264) on the texture object: called with the
// listbox itself as its single argument.
// Virtual slot 0x000 on the previous texture object: destructor-style call with flag 1.

static int __fastcall GuiEffect_SetListBoxTexture(void* self, void* /*edx*/, int newTexture)
{
    int result = 0;

    // Release the previously attached texture object, if any.
    void* oldTexture = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x134));
    if (oldTexture) {
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, oldTexture, 0)[0])(oldTexture, nullptr, 1);
        CSF_AT(uint32_t, self, 0x134) = 0;
    }

    // Attach the new texture.
    CSF_AT(uint32_t, self, 0x134) = static_cast<uint32_t>(newTexture);

    if (newTexture) {
        uint8_t flag = CSF_AT(uint8_t, self, 0x1f4);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, reinterpret_cast<void*>(newTexture), 0)[640 / 4])(
            reinterpret_cast<void*>(newTexture), nullptr, static_cast<int>(flag));

        void* attached = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x134));
        return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, attached, 0)[264 / 4])(
            attached, nullptr, self);
    }

    return result;
}

HOOK(0x714050, GuiEffect_SetListBoxTexture, "auto");
