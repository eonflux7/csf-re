#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ForwardToAttachedObject, 0x0048E5F0, (void* self, void* edx, void* owner));
CSF_FUNC(int, __fastcall, ClearAttachedObjectState, 0x00493270, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ProcessAttachedObjectFlag, 0x004E1F10, (void* self, void* edx));

// Dispatches on the object's state code (+0x18). States 2 and 4 are gated by a flag at +0x1C or
// by the attached object's byte at +0x228; state 2 tail-jumps to a virtual slot of this object.
static hook::Tail __fastcall DispatchByState(void* self, const uint32_t* /*args*/)
{
    C_0083e5e8* object = static_cast<C_0083e5e8*>(self);
    uint8_t* attached = reinterpret_cast<uint8_t*>(object->field_000c);
    uint32_t state = CSF_AT(uint32_t, object, 0x18);

    if (state == 2)
    {
        uint8_t flag = CSF_AT(uint8_t, object, 0x1c);
        if (flag != 0)
            return hook::tail_ret(static_cast<uint32_t>(flag));
        return hook::tail_jump(self, (*reinterpret_cast<void***>(self))[0xD0 / 4]);
    }

    if (state == 3)
    {
        if (CSF_AT(uint8_t, attached, 0x228) != 0)
        {
            ForwardToAttachedObject(attached, nullptr, self);
            int result = ClearAttachedObjectState(attached, nullptr);
            CSF_AT(uint32_t, object, 0x18) = 0;
            return hook::tail_ret(static_cast<uint32_t>(result));
        }
        return hook::tail_ret(0);
    }

    if (state == 4)
    {
        if (CSF_AT(uint8_t, attached, 0x228) != 0)
        {
            uint8_t flagResult = static_cast<uint8_t>(ProcessAttachedObjectFlag(self, nullptr));
            return hook::tail_ret(static_cast<uint32_t>(flagResult));
        }
        return hook::tail_ret(0);
    }

    return hook::tail_ret(state - 4);
}

HOOK_TAIL(0x004E20B0, DispatchByState, "auto");
