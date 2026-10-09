#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable of the class this method belongs to (referenced by the original at 0x48830C).
[[maybe_unused]] static const auto s_S00486c30Vtbl = CSF_VTBL(0x00486C30);

// Callee: the original's `call 0x486c30`, returns int, takes (this, float a2).
CSF_FUNC(int, __fastcall, S_00486c30_RecomputeFromFloat, 0x00486C30, (void* self, void* edx, float a2));

// Calls the object's virtual slot 0xA4 (index 41) with (a2 bits, a3), then recomputes from a2.
static int __fastcall S_00486c30_NotifyThenRecompute(S_00486c30* self, void* /*edx*/, float a2, int a3)
{
    // The vtable pointer is the first dword of the object (field_0000).
    using NotifyFn = void(__fastcall*)(void* self, void* edx, uint32_t a2Bits, int a3);
    NotifyFn notify = reinterpret_cast<NotifyFn>(CSF_AT(void**, self, 0)[0xA4 / 4]);

    uint32_t a2Bits = 0;
    std::memcpy(&a2Bits, &a2, sizeof(a2Bits));
    notify(self, nullptr, a2Bits, a3);

    return S_00486c30_RecomputeFromFloat(self, nullptr, a2);
}

HOOK(0x004882F0, S_00486c30_NotifyThenRecompute, "auto");
