#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, ResetAndReleaseObjectSlots, 0x00525060, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the scene loader's settings service: releases the three owned sub-objects
// (vtable slot 19 = Release), clears the cached buffer at +0x8C, and sets the
// default range values at +0x60..+0x88 to 0x7EFFFFFF.
static int __fastcall SceneLoader_ResetSettingsService(void* self, void* /*edx*/)
{
    ResetAndReleaseObjectSlots(static_cast<uint8_t*>(self) + 0x28, nullptr);

    // Owned object at +0x0C
    void* ownedAtC = CSF_AT(void*, self, 0x0C);
    if (ownedAtC != nullptr) {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, ownedAtC, 0)[19])(ownedAtC, nullptr);
        CSF_AT(void*, self, 0x0C) = nullptr;
    }

    // Owned object at +0x08
    void* ownedAt8 = CSF_AT(void*, self, 0x08);
    if (ownedAt8 != nullptr) {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, ownedAt8, 0)[19])(ownedAt8, nullptr);
        CSF_AT(void*, self, 0x08) = nullptr;
    }

    // Owned object at +0x10
    void* ownedAt10 = CSF_AT(void*, self, 0x10);
    if (ownedAt10 != nullptr) {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, ownedAt10, 0)[19])(ownedAt10, nullptr);
        CSF_AT(void*, self, 0x10) = nullptr;
    }

    CSF_AT(uint32_t, self, 0x18) = 0;

    void* buffer = CSF_AT(void*, self, 0x8C);
    CSF_AT(uint32_t, self, 0x90) = 0;
    CSF_AT(uint32_t, self, 0x94) = 0;
    if (buffer != nullptr) {
        Mem_Free(buffer);
        CSF_AT(void*, self, 0x8C) = nullptr;
    }

    const uint32_t defaultRange = 0x7EFFFFFF;
    CSF_AT(uint32_t, self, 0x60) = defaultRange;
    CSF_AT(uint32_t, self, 0x64) = defaultRange;
    CSF_AT(uint32_t, self, 0x68) = defaultRange;
    CSF_AT(uint32_t, self, 0x6C) = defaultRange;
    CSF_AT(uint32_t, self, 0x70) = defaultRange;
    CSF_AT(uint32_t, self, 0x74) = defaultRange;
    CSF_AT(uint32_t, self, 0x78) = defaultRange;
    CSF_AT(uint32_t, self, 0x7C) = defaultRange;
    CSF_AT(uint32_t, self, 0x80) = defaultRange;
    CSF_AT(uint32_t, self, 0x84) = defaultRange;
    CSF_AT(uint32_t, self, 0x88) = defaultRange;

    return 2130706431; // 0x7EFFFFFF
}

HOOK(0x00525970, SceneLoader_ResetSettingsService, "auto");
