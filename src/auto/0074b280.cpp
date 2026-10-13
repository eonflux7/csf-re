#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));

static void* __fastcall NetGhostDDL_WriteObjectTypeByteToConfig(C_008622e8* self, void* /*edx*/, C_008622e8* a2, int a3, int a4, int a5)
{
    // The byte at offset 0 of the object (low byte of its vtable pointer).
    char objectTypeByte = static_cast<char>(CSF_AT(uint8_t, self, 0));
    (void)a3;
    (void)a4;
    (void)a5;
    return GameConfig_WriteByteField(a2, nullptr, objectTypeByte);
}

HOOK(0x74B280, NetGhostDDL_WriteObjectTypeByteToConfig, "auto");
