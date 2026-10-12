#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Interface module, INSTANCING SCENE/RESULT: stores the byte flag at +0x181 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x181], al; ret 4 (returns the value in al only).
static char __fastcall Interface_SetInstancingResultFlag(void* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x181) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x537240, Interface_SetInstancingResultFlag, "auto");
