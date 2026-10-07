#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::uint32_t, g_CameraClearColor, 0x008BB430);

CSF_FUNC(int, __cdecl, RwCameraClear, 0x00628F60,
         (int camera, int color, int flags));
CSF_FUNC(int, __cdecl, RwCameraBeginUpdate, 0x00628E50,
         (int camera));

static bool __fastcall C_008655e8_ClearAndBeginCameraUpdate(
    C_008655e8* self, void* /*edx*/, char clearColor)
{
    if (RwCameraClear(
            static_cast<int>(self->field_000c),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(&g_CameraClearColor)),
            clearColor ? 3 : 2) == 0)
    {
        return false;
    }

    return RwCameraBeginUpdate(static_cast<int>(self->field_000c)) != 0;
}

HOOK(0x00726A80, C_008655e8_ClearAndBeginCameraUpdate, "auto");
