#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable of the class this method belongs to (referenced by the original at 0x4882DC).
[[maybe_unused]] static const auto s_S00486c30Vtbl = CSF_VTBL(0x00486C30);

// Callee: the original's `call 0x486c30`, returns int, takes (this, float a2).
CSF_FUNC(int, __fastcall, S_00486c30_RecomputeFromFloat, 0x00486C30, (void* self, void* edx, float a2));

// Notifies the object through vtable slot 0xA8 (index 42), then recomputes from a2.
static int __fastcall S_00486c30_NotifyAndRecompute(S_00486c30* self, void* /*edx*/, float a2, int a3)
{
    // Vtable slot at byte offset 0xA8 -> index 42. Passes (this, a2 bits, a3).
    using NotifyFn = void(__fastcall*)(void* self, void* edx, uint32_t a2Bits, int a3);
    NotifyFn notify = reinterpret_cast<NotifyFn>(CSF_AT(void**, self, 0)[0xA8 / 4]);
    notify(self, nullptr, static_cast<uint32_t>(*reinterpret_cast<uint32_t*>(&a2)), a3);

    return S_00486c30_RecomputeFromFloat(self, nullptr, a2);
}

HOOK(0x004882C0, S_00486c30_NotifyAndRecompute, "auto");
