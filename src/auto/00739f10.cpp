#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, DestroyTreeBranch, 0x00739B80,
         (void* tree, void* edx, void** block));

static int __fastcall C_00867444_ResetState(
    C_00867444* self, void* /*edx*/)
{
    const uint32_t entryCount = self->field_003c;
    for (uint32_t index = 0; index < entryCount; ++index) {
        auto* entries = reinterpret_cast<uint32_t*>(self->field_0034);
        auto* owner = reinterpret_cast<void*>(self->field_0008);
        const uint32_t entry = entries[index];
        void** vtable = CSF_AT(void**, owner, 0);

        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            vtable[0x88 / 4])(owner, nullptr, entry);
    }

    self->field_003c = 0;

    auto* sentinel = reinterpret_cast<void*>(self->field_002c);
    void** firstNode = CSF_AT(void**, sentinel, 0x04);
    DestroyTreeBranch(reinterpret_cast<uint8_t*>(self) + 0x28,
                   nullptr, firstNode);

    sentinel = reinterpret_cast<void*>(self->field_002c);
    CSF_AT(uint32_t, sentinel, 0x04) =
        reinterpret_cast<uint32_t>(sentinel);

    sentinel = reinterpret_cast<void*>(self->field_002c);
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, sentinel, 0x00) =
        reinterpret_cast<uint32_t>(sentinel);

    sentinel = reinterpret_cast<void*>(self->field_002c);
    CSF_AT(uint32_t, sentinel, 0x08) =
        reinterpret_cast<uint32_t>(sentinel);

    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_005c = 0;
    self->field_005d = 0;
    self->field_0054 = 1.0f;
    self->field_0058 = 1.0f;
    self->field_005e = 1;

    return 0x3F800000;
}

HOOK(0x00739F10, C_00867444_ResetState, "auto");
