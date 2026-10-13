#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

// Updates the owner child at +0x30, then the slot child at +0xD0 when an active slot exists
// (slots at +0x6C, stride 0x20, 4 entries), then draws the widget at +0x44.
static char __fastcall Hud_RenderSubtitlePanel(uint8_t* self, void* /*edx*/)
{
    uint32_t lastResult = 0;

    if (CSF_AT(uint8_t, self, 0x4D) == 0)
        return 0;

    // Owner child at +0x30 gets its vtable slot 2 called when both +0x30 and +0x3C are set.
    void* owner = CSF_AT(void*, self, 0x30);
    if (owner != nullptr && CSF_AT(void*, self, 0x3C) != nullptr)
    {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[2])(owner, nullptr);
    }

    lastResult = CSF_AT(uint32_t, self, 0xD0);
    if (lastResult != 0)
    {
        // Scan the 4 slots at +0x6C (stride 0x20) for an active entry.
        uint32_t slotIndex = 0;
        uint8_t* slotFlag = self + 0x6C;
        bool slotFound = false;
        while (slotIndex < 4)
        {
            if (*slotFlag != 0)
            {
                slotFound = true;
                break;
            }
            ++slotIndex;
            slotFlag += 0x20;
        }

        lastResult = slotIndex;
        if (slotFound)
        {
            void* child = CSF_AT(void*, self, 0xD0);
            lastResult = (uint32_t)((int(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[2])(child, nullptr);
        }
    }

    void* widget = CSF_AT(void*, self, 0x44);
    if (widget != nullptr)
    {
        Hud_RenderWidgetAtScaledRect(widget, nullptr);
    }

    return (char)lastResult;
}

HOOK(0x439CF0, Hud_RenderSubtitlePanel, "auto");
