#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// int __cdecl RwCameraEndUpdate(int camera)  -- address 0x00628E40
CSF_FUNC(int, __cdecl, RwCameraEndUpdate, 0x628E40, (int camera));

// 0x006D2BE0: ends the camera update for the camera stored at +0x0C.
static int __fastcall C_008655e8_EndCameraUpdate(C_008655e8* self, void* /*edx*/)
{
    return RwCameraEndUpdate(static_cast<int>(self->field_000c));
}

HOOK(0x6D2BE0, C_008655e8_EndCameraUpdate, "auto");
