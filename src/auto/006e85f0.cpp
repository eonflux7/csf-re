#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// The function stores the 32-bit argument into the DWORD at this+0x54 and
// returns it unchanged. The declared field at 0x54 is a float, so the store is
// performed through a uint32_t reinterpretation to keep the exact bit pattern.
static int __fastcall C_00861e98_StoreValue54(C_00861e98* self, void* /*edx*/, int value)
{
    *reinterpret_cast<uint32_t*>(&self->field_0054) = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x006E85F0, C_00861e98_StoreValue54, "auto");
