#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, FindIconIndex, 0x00458980, (void* self, void* edx, float* a2));
CSF_FUNC(void**, __fastcall, AppendIconRecord, 0x00458DD0, (void* self, void* edx, void** a2, char* a3));

// Returns true when the icon at the matched index was found and its record was appended.
static bool __fastcall HudStatusIcons_TryRegisterIcon(S_00458980* self, void* /*edx*/, float* a2)
{
    uint32_t listBegin = self->field_002c;
    if (!listBegin)
        return false;

    // Signed count of 20-byte slots between the list begin and end.
    int slotCount = (int)(self->field_0030 - listBegin) / 20;
    if (!slotCount)
        return false;

    int entryIndex = FindIconIndex(self, nullptr, a2);
    if (entryIndex == -1)
        return false;

    // Each icon record is 40 bytes, starting at field_001c.
    char* entry = reinterpret_cast<char*>(self->field_001c + 40u * static_cast<uint32_t>(entryIndex));

    AppendIconRecord(reinterpret_cast<uint8_t*>(self) + 0x18, nullptr,
                     reinterpret_cast<void**>(&a2), entry);
    return true;
}

HOOK(0x00458ED0, HudStatusIcons_TryRegisterIcon, "auto");
