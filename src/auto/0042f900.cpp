#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint64_t, __cdecl, Hud_ComputeOverlayCoords, 0x0042F3B0, (int* a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Refreshes the "G" overlay text of the HUD when the global debug flag is set.
// Returns 0 if the overlay object is absent, the global pointer if the flag is clear,
// otherwise the result of the final virtual call.
static int __fastcall Hud_RefreshDebugOverlay(uint8_t* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x240);
    if (result)
    {
        result = GetGlobalValue_8BC1E0();
        if (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(result), 0x90C))
        {
            void* overlay = CSF_AT(void*, self, 0x240);
            ((void(__fastcall*)(void*, void*, const char*))CSF_AT(void**, overlay, 0)[15])
                (overlay, nullptr, CSF_STR(0x830914, "G"));

            int coords[2];
            coords[0] = 230;
            coords[1] = 530;
            Hud_ComputeOverlayCoords(coords);

            overlay = CSF_AT(void*, self, 0x240);
            ((void(__fastcall*)(void*, void*, int*))CSF_AT(void**, overlay, 0)[20])
                (overlay, nullptr, coords);

            overlay = CSF_AT(void*, self, 0x240);
            return ((int(__fastcall*)(void*, void*))CSF_AT(void**, overlay, 0)[2])
                (overlay, nullptr);
        }
    }
    return result;
}

HOOK(0x0042F900, Hud_RefreshDebugOverlay, "auto");
