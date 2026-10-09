#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, Mapping_AppendEntry, 0x00455470,
         (void* self, void* edx, uint32_t* a3));

static void __fastcall Game_SetMappedValue(
    void* self, void* /*edx*/, const void* key, const void* value)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x110);
    uint32_t count = 0;
    if (begin) {
        const uint32_t end = CSF_AT(uint32_t, self, 0x114);
        count = static_cast<uint32_t>(
            static_cast<int32_t>(end - begin) / 24);
    }

    for (uint32_t index = 0; index < count; ++index) {
        auto* entries = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x110)));
        if (CSF_AT(uint32_t, key, 0x4) ==
            CSF_AT(uint32_t, entries + index * 24, 0x4)) {
            auto* entry = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x110)))
                + index * 24;
            CSF_AT(uint32_t, entry, 0x10) = CSF_AT(uint32_t, value, 0x4);
            CSF_AT(uint32_t, entry, 0x14) = CSF_AT(uint32_t, value, 0x8);
            return;
        }
    }

    uint32_t entry[8]{};
    entry[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    entry[3] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    entry[1] = CSF_AT(uint32_t, key, 0x4);
    entry[2] = CSF_AT(uint32_t, key, 0x8);
    entry[4] = CSF_AT(uint32_t, value, 0x4);
    entry[5] = CSF_AT(uint32_t, value, 0x8);
    Mapping_AppendEntry(static_cast<uint8_t*>(self) + 0x10C, nullptr, entry);
}

HOOK(0x00455DF0, Game_SetMappedValue, "auto");
