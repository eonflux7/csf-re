// Module: hud_alert_icons (gfx/iconos.txt, ALERTA/DUDOSO/PLANO icons)
#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// _memcpy (CRT), __cdecl, 3 stack args, pops 0.
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Container layout (from this): +0x04 = entry array base, +0x0C = entry count.
// Entries are 12 bytes; the 32-bit key lives at entry +0x04.
static uint32_t __fastcall HudAlertIcons_RemoveEntryMatchingKey(uint32_t* self, void* /*edx*/, const void* entry)
{
    uint32_t result = 0;

    if (CSF_AT(uint32_t, self, 0x0C) != 0)
    {
        uint32_t searchKey = CSF_AT(uint32_t, entry, 0x04);

        // Linear search for the first entry whose key matches.
        for (;;)
        {
            uint8_t* base = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04));
            uint32_t currentKey = *reinterpret_cast<uint32_t*>(base + 12 * result + 4);
            if (currentKey == searchKey)
                break;
            ++result;
            if (result >= CSF_AT(uint32_t, self, 0x0C))
                return result;
        }

        if (result != 0xFFFFFFFFu)
        {
            // Remove the matched entry by shifting the tail down one slot.
            uint32_t newCount = CSF_AT(uint32_t, self, 0x0C) - 1;
            CSF_AT(uint32_t, self, 0x0C) = newCount;

            if (result != newCount)
            {
                uint8_t* base = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04));
                uint8_t* dst = base + 12 * result;
                const uint8_t* src = dst + 12;
                std::size_t tailBytes = 12 * (newCount - result);
                return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(_memcpy(dst, src, tailBytes)));
            }
        }
    }

    return result;
}

HOOK(0x4A0F50, HudAlertIcons_RemoveEntryMatchingKey, "auto");
