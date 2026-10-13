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

// Callees of 0x004A97A0, bound by address.
CSF_FUNC(int, __fastcall, IsField90Null, 0x0040C910, (void* self, void*));
CSF_FUNC(int, __fastcall, FinalizeActiveState, 0x004A9140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));

// Virtual slot 0xD8 (index 54) of C_00835228: returns a bool test.
static bool C_00835228_IsSlotD8Active(C_00835228* self)
{
    using SlotD8Fn = bool(__fastcall*)(void* self, void* edx);
    return ((SlotD8Fn)CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
}

// Called when the target object at field_0014->field_0044 must be notified
// (vtable slot 0xA0 of that object's type).
static void C_00835228_NotifyFieldObject(void* holder)
{
    void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, holder, 0x44));
    using NotifyFn = void(__fastcall*)(void* self, void* edx);
    ((NotifyFn)CSF_AT(void**, target, 0)[0xA0 / 4])(target, nullptr);
}

static int __fastcall C_00835228_ProcessState(C_00835228* self, void* /*edx*/, int a2)
{
    (void)a2;
    void* holder = reinterpret_cast<void*>(self->field_0014);
    int result;

    if (C_00835228_IsSlotD8Active(self))
    {
        uint32_t fieldObject = CSF_AT(uint32_t, holder, 0x4C);
        result = IsField90Null(reinterpret_cast<void*>(fieldObject), nullptr);
        if ((uint8_t)result)
        {
            if (!self->field_0070)
                C_00835228_NotifyFieldObject(holder);
            self->field_0070 = 2;
            return FinalizeActiveState(self, nullptr);
        }
    }
    else
    {
        if (!self->field_0070)
            C_00835228_NotifyFieldObject(holder);
        self->field_0070 = 4;
        result = ResetState(reinterpret_cast<uint8_t*>(self) + 0x1C, nullptr);
        self->field_0018 &= ~1u;
    }
    return result;
}

HOOK(0x004A97A0, C_00835228_ProcessState, "auto");
