#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, Hud_SetWidgetParam, 0x006C7B40, (int a1, int a2));
CSF_FUNC(void*, __fastcall, S_0042ee00_PlaceDebugRow, 0x0042F0E0, (void* self, void* edx, float a2));

static char __fastcall S_0042ee00_RefreshDebugOverlay(S_0042ee00* self, void* /*edx*/)
{
    if (self->field_023c == 0)
        return 0;
    if (self->field_011f == 0)
        return 0;
    if (self->field_0234 == 0)
        return 0;

    // The original loads a pointer from GetGlobalValue()+4 and leaves it in esi,
    // which it also places in ecx before each Hud_SetWidgetParam call.
    // Hud_SetWidgetParam is bound as __stdcall, so ecx is not forwarded here.
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_6C5760()));
    void* hudState = CSF_AT(void*, globalValue, 4);
    (void)hudState;

    Hud_SetWidgetParam(20, 1);
    Hud_SetWidgetParam(6, 0);
    Hud_SetWidgetParam(8, 0);
    Hud_SetWidgetParam(29, 5);
    Hud_SetWidgetParam(30, 0);
    Hud_SetWidgetParam(10, 5);
    Hud_SetWidgetParam(11, 6);

    // Re-read after the calls above; the callees may have changed the object.
    uint32_t rowsLeft = self->field_0234;
    if (rowsLeft != 0)
    {
        uint32_t rowOffset = 0;
        do
        {
            // The original pushes the integer sum and the callee receives its bits as a float.
            uint32_t offsetBits = rowOffset + self->field_022c;
            float rowY;
            std::memcpy(&rowY, &offsetBits, sizeof(rowY));
            S_0042ee00_PlaceDebugRow(self, nullptr, rowY);
            rowOffset += 16;
            --rowsLeft;
        } while (rowsLeft != 0);
    }

    Hud_SetWidgetParam(20, 2);
    Hud_SetWidgetParam(6, 1);
    Hud_SetWidgetParam(8, 1);
    return static_cast<char>(Hud_SetWidgetParam(29, 8));
}

HOOK(0x00431A30, S_0042ee00_RefreshDebugOverlay, "auto");
