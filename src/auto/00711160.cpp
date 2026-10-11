#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Virtual slots on the object held in field_0004 (byte offset / 4):
//   +0x30 (slot 12): query/apply with (flag, 0, 0) and returns an HRESULT-style int
//   +0x34 (slot 13): set a state value with (0)
//   +0x50 (slot 20): recovery call, returns nonzero on success

static void C_00863cd4_ApplyFlagWithDsRetry_CallSlot13(void* obj, int value)
{
    using Slot13Fn = void(__stdcall*)(void*, int);
    ((Slot13Fn)CSF_AT(void**, obj, 0)[13])(obj, value);
}

static int C_00863cd4_ApplyFlagWithDsRetry_CallSlot12(void* obj, int a, int b, int c)
{
    using Slot12Fn = int(__stdcall*)(void*, int, int, int);
    return ((Slot12Fn)CSF_AT(void**, obj, 0)[12])(obj, a, b, c);
}

static int C_00863cd4_ApplyFlagWithDsRetry_CallSlot20(void* obj)
{
    using Slot20Fn = int(__stdcall*)(void*);
    return ((Slot20Fn)CSF_AT(void**, obj, 0)[20])(obj);
}

static int __fastcall C_00863cd4_ApplyFlagWithDsRetry(C_00863cd4* self, void* /*edx*/, char a2)
{
    C_00863cd4_ApplyFlagWithDsRetry_CallSlot13(reinterpret_cast<void*>(self->field_0004), 0);

    int flagValue = (a2 != 0) ? 1 : 0;
    int result = C_00863cd4_ApplyFlagWithDsRetry_CallSlot12(
        reinterpret_cast<void*>(self->field_0004), flagValue, 0, 0);

    if (result == -2005401450)
    {
        result = C_00863cd4_ApplyFlagWithDsRetry_CallSlot20(
            reinterpret_cast<void*>(self->field_0004));
        if (result == 0)
        {
            return C_00863cd4_ApplyFlagWithDsRetry_CallSlot12(
                reinterpret_cast<void*>(self->field_0004), flagValue, 0, 0);
        }
    }
    return result;
}

HOOK(0x00711160, C_00863cd4_ApplyFlagWithDsRetry, "auto");
