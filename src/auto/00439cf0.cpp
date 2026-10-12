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

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

// Hud_RenderSubtitlePanel: renders the subtitle panel. Ticks the first child widget
// if it is visible and an owner is set, then runs the second child's update slot
// (its return low byte becomes the result), then draws the attached widget.
static char __fastcall Hud_RenderSubtitlePanel(void* self, void* /*edx*/)
{
    // Low byte of the function's return value (the original's `al`).
    uint8_t resultByte = CSF_AT(uint8_t, self, 0x4d);

    if (resultByte)
    {
        // First child object at +0x30: call its slot 2 if +0x3C is set.
        void* firstChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x30));
        if (firstChild && CSF_AT(uint32_t, self, 0x3C))
        {
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, firstChild, 0)[2])(firstChild, nullptr);
        }

        // Second child object at +0xD0: only if one of the four 32-byte slots starting at +0x6C is clear.
        void* secondChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0xD0));
        if (secondChild)
        {
            uint32_t slotIndex = 0;
            uint8_t* slotFlag = reinterpret_cast<uint8_t*>(self) + 0x6C;
            bool foundClearSlot = false;
            while (slotIndex < 4)
            {
                if (*slotFlag == 0)
                {
                    foundClearSlot = true;
                    break;
                }
                ++slotIndex;
                slotFlag += 0x20;
            }

            if (foundClearSlot)
            {
                // v2 == slotIndex here; the call's returned int supplies the low byte.
                resultByte = static_cast<uint8_t>(
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, secondChild, 0)[2])(secondChild, nullptr));
            }
            else
            {
                // Loop ran to completion: eax == 4 falls through as the result.
                resultByte = 4;
            }
        }

        // Attached widget at +0x44: draw it with a null rect.
        void* widget = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x44));
        if (widget)
        {
            Hud_RenderWidgetAtScaledRect(widget, nullptr);
            resultByte = 0; // callee is void; its eax is undefined, so pass 0
        }
    }

    return static_cast<char>(resultByte);
}

HOOK(0x439CF0, Hud_RenderSubtitlePanel, "auto");
