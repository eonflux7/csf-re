#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

// Refreshes the pending-flag bits in S_00493c70 from the sub-object at +0x44.
// Clears bit 0x10 of field_02e8 when field_02e0 has bit 0x08 set; otherwise clears
// the low two bits of field_02e8 when the sub-object reports state 3.
static C_008319c0* __fastcall S_00493c70_RefreshPendingFlags(S_00493c70* self, void* /*edx*/)
{
    void* subObject = reinterpret_cast<void*>(self->field_0044);

    // vtable slot 216 / 4 = 54
    C_008319c0* result =
        ((C_008319c0* (__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[216 / 4])(subObject, nullptr);

    if (result) {
        int subFieldValue = C_008319c0_GetSubObjectField74(result, nullptr);
        result = reinterpret_cast<C_008319c0*>(static_cast<intptr_t>(subFieldValue));

        if (subFieldValue == 8) {
            if (self->field_02e0 & 8) {
                uint32_t masked = self->field_02e8 & 0xFFFFFFEFu;
                self->field_02e8 = masked;
                result = reinterpret_cast<C_008319c0*>(static_cast<uintptr_t>(masked));
            } else {
                // vtable slot 244 / 4 = 61
                result =
                    ((C_008319c0* (__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[244 / 4])(subObject, nullptr);
                if (reinterpret_cast<intptr_t>(result) == 3) {
                    self->field_02e8 &= 0xFFFFFFFCu;
                }
            }
        }
    }
    return result;
}

HOOK(0x004948D0, S_00493c70_RefreshPendingFlags, "auto");
