#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, MemDebug_FreeDebugBlock, 0x00690E60, (void*));
CSF_FUNC(int, __fastcall, AppConfig_DestroyStorage, 0x0075FE80, (void*, void*));
static int __fastcall C_0086c2b4_Destroy(C_0086c2b4* self, void*) {
    self->vtbl = CSF_VTBL(0x0086C2B4);
    if (self->field_012c >= 16 && self->field_0118)
        MemDebug_FreeDebugBlock(reinterpret_cast<void*>(self->field_0118));
    self->field_012c = 15;
    self->field_0128 = 0;
    CSF_AT(uint8_t, self, 0x118) = 0;
    int result = AppConfig_DestroyStorage(reinterpret_cast<char*>(self)+8, nullptr);
    self->vtbl = CSF_VTBL(0x008446CC);
    return result;
}
HOOK(0x0075FFE0, C_0086c2b4_Destroy, "auto");
