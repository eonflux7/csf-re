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
#include "legacy/string.h"

// Bound callees (addresses from the packet).
CSF_FUNC(bool, __fastcall, IsSlotTwelveActive, 0x00489C30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));

// Virtual slot 0xBC (= 47 * 4) on the object at field_0014->field_0044, called with arg 12.
static int __fastcall CallSlotBcWithTwelve(void* target)
{
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, target, 0)[0xBC / 4])(target, nullptr, 12);
}

static void __fastcall CallSlotBcWithTwelveVoid(void* target)
{
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, target, 0)[0xBC / 4])(target, nullptr, 12);
}

// 0x004DAD00: __thiscall, one stack argument (a2, unused), ret 4.
static char __fastcall C_0083dd58_ProcessSlotTwelve(C_0083dd58* self, void* /*edx*/, int /*a2*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* v3 = reinterpret_cast<void*>((uintptr_t)CSF_AT(uint32_t, owner, 0x44));
    int v4 = CallSlotBcWithTwelve(v3);
    bool slotActive = IsSlotTwelveActive(v3, nullptr, v4);
    if (!slotActive)
        return 0;

    if (self->field_0070 == 0)
    {
        // Re-read the object pointer; the original reloads it here.
        void* owner2 = reinterpret_cast<void*>(self->field_0014);
        void* target = reinterpret_cast<void*>((uintptr_t)CSF_AT(uint32_t, owner2, 0x44));
        CallSlotBcWithTwelveVoid(target);
    }

    self->field_0070 = 5;
    int resetResult = ResetState(&self->field_001c, nullptr);
    self->field_0018 &= ~1u;
    return static_cast<char>(resetResult);
}

HOOK(0x004DAD00, C_0083dd58_ProcessSlotTwelve, "auto");
