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

// Virtual slot 0x2AC (171): returns int, no args, __thiscall.
// Slot 0x3C (15): takes one int arg, __thiscall, pops 4 bytes.
// Both are called through the object's vtable; the callee's `ret` is not
// reproduced here because the caller-side vtable dispatch cleans up the stack.

static int __fastcall CMenuSoundOptionsPC_SyncXFiOptionState(CMenuSoundOptionsPC* self, void* /*edx*/)
{
    // First-stage check: the object at +0x2C must be present.
    int result = static_cast<int>(self->field_002c);
    if (result == 0)
        return result;

    // Second-stage check: the X-Fi helper object at +0x5C must be present.
    result = static_cast<int>(self->field_005c);
    if (result == 0)
        return result;

    // Snapshot the vtable of the +0x2C object before the first call (mov edi, [eax]).
    void* targetObj = reinterpret_cast<void*>(self->field_002c);
    void** targetVtbl = CSF_AT(void**, targetObj, 0);
    void* targetVtblPtr = reinterpret_cast<void*>(targetVtbl);
    (void)targetVtblPtr;

    // Call slot 0x2AC on the +0x5C object (ecx = that object).
    void* xfiObj = reinterpret_cast<void*>(self->field_005c);
    int queryResult =
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, xfiObj, 0)[0x2AC / 4])(xfiObj, nullptr);

    // neg eax ; sbb al,al ; inc al  -> al = (queryResult == 0), upper bits = -queryResult.
    uint32_t negated = 0u - static_cast<uint32_t>(queryResult);
    uint8_t lowByte = (queryResult != 0) ? static_cast<uint8_t>(0xFF) : static_cast<uint8_t>(0x00);
    lowByte = static_cast<uint8_t>(lowByte + 1);
    uint32_t pushedValue = (negated & 0xFFFFFF00u) | static_cast<uint32_t>(lowByte);

    // Reload the +0x2C object (mov ecx, [esi+0x2c]) and call slot 0x3C on it.
    void* ownerObj = reinterpret_cast<void*>(self->field_002c);
    return ((int(__fastcall*)(void*, void*, int))targetVtbl[0x3C / 4])(
        ownerObj, nullptr, static_cast<int>(pushedValue));
}

HOOK(0x548B10, CMenuSoundOptionsPC_SyncXFiOptionState, "auto");
