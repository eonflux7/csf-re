#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
static CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008318c8_IsWeaponSubObjectReady(
    C_008318c8* self, void* /*edx*/)
{
    if ((self->field_006c & 0x80000000) != 0)
        return 0;

    return CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x64));
}

HOOK(0x00478280, C_008318c8_IsWeaponSubObjectReady, "auto");
