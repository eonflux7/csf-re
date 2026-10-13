#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, StringFormat_ConvertWideTextToNarrow, 0x00695620,
         (const uint16_t* wideText, uint8_t* narrowOut, unsigned int maxChars));

// __thiscall int C_008442a0::ConvertAndForwardText(int wideText, int arg)
// Converts the wide text into a 256-byte narrow scratch buffer (result unused),
// then forwards the call to the attached child at +0xA4 (vtable slot 27),
// or returns 1 when no child is attached.
static int __fastcall C_008442a0_ConvertAndForwardText(C_008442a0* self, void* /*edx*/,
                                                       int a2, int a3) {
    alignas(8) uint8_t narrowBuffer[256];

    StringFormat_ConvertWideTextToNarrow(reinterpret_cast<const uint16_t*>(a2),
                                         narrowBuffer, 256);

    void* child = reinterpret_cast<void*>(self->field_00a4);
    if (child) {
        return ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, child, 0)[108 / 4])
            (child, nullptr, a2, a3);
    }
    return 1;
}

HOOK(0x0054AD80, C_008442a0_ConvertAndForwardText, "auto");
