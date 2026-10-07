#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the byte located at offset +1 of the object referenced by this->field_00bc.
// Original: mov eax,[ecx+0xbc]; mov al,[eax+1]; ret
static char __fastcall C_00860bb8_GetStateByte1(C_00860bb8* self, void* /*edx*/)
{
    const uint8_t* stateObject = reinterpret_cast<const uint8_t*>(self->field_00bc);
    return static_cast<char>(stateObject[1]);
}

HOOK(0x6CACC0, C_00860bb8_GetStateByte1, "auto");
