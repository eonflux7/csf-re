#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, StringFormat_ConvertWideTextToNarrow, 0x00695620,
         (const uint16_t* wideText, uint8_t* narrowOut, unsigned int maxChars));

// Forwards the three arguments to the child object's virtual slot 29 (byte offset 0x74),
// then converts the wide text a2 into a 256-byte scratch buffer. Returns the child's result,
// or 1 when there is no child object.
static char __fastcall C_008442a0_ForwardWideTextAndConvert(C_008442a0* self, void* /*edx*/,
                                                            int a2, int a3, int a4)
{
    if (self->field_00a4 == 0)
        return 1;

    void* childObject = reinterpret_cast<void*>(self->field_00a4);
    char childResult = ((char(__fastcall*)(void*, void*, int, int, int))
                            CSF_AT(void**, childObject, 0)[0x74 / 4])(childObject, nullptr, a2, a3, a4);

    alignas(8) uint8_t narrowBuffer[0x100];
    StringFormat_ConvertWideTextToNarrow(reinterpret_cast<const uint16_t*>(a2), narrowBuffer, 256);

    return childResult;
}

HOOK(0x0054ADF0, C_008442a0_ForwardWideTextAndConvert, "auto");
