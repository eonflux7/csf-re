#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callee: CharTask_LookupByOffset (__thiscall, 1 stack arg, ret 4).
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// TDisparar::GetName
// Refreshes the owner's sub-object (vtable slot 5), then looks up the task
// entry at (this->field_000c + 0x24) relative to this.
static int __fastcall TDisparar_GetName(TDisparar* self, void* /*edx*/, int /*a2*/, int /*a3*/) {
    // Owner pointer stored at +0x08; its sub-object pointer lives at +0x14.
    uint32_t owner = CSF_AT(uint32_t, self, 0x08);
    uint32_t ownerInner = CSF_AT(uint32_t, reinterpret_cast<void*>(owner), 0x14);

    // Embedded sub-object at ownerInner + 4 (lea, not a pointer load).
    void* refreshTarget = reinterpret_cast<void*>(ownerInner + 4);

    // Virtual call: vtable slot 0x14 / 4 = 5, no arguments.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, refreshTarget, 0)[5])(refreshTarget, nullptr);

    // Lookup using the offset stored at +0x0C plus 0x24.
    uint32_t taskOffset = CSF_AT(uint32_t, self, 0x0C);
    return CharTask_LookupByOffset(self, nullptr, static_cast<int>(taskOffset + 0x24));
}

HOOK(0x0047B530, TDisparar_GetName, "auto");
