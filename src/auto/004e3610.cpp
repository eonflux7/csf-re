#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_493270, 0x00493270, (void* self, void* edx));

// Resets the state machine of C_0083e248 to idle (1). If the state was 3, first
// flags the object at +0x0C and invokes its sub-object's slot 0x50 with a flag byte.
// Then forwards a request (48, 0, -1, 0) through the +0x0C object's vtable slot 0x8C.
static int __fastcall C_0083e248_ResetStateToIdle(C_0083e248* self, void* /*edx*/)
{
    if (self->field_0018 == 3)
    {
        sub_493270(reinterpret_cast<void*>(self->field_000c), nullptr);

        void* owner = reinterpret_cast<void*>(self->field_000c);
        uint32_t innerAddr = CSF_AT(uint32_t, owner, 0x34);
        CSF_AT(uint32_t, reinterpret_cast<void*>(innerAddr), 0x18) |= 2u;

        char flag = 0;
        void* subObj = reinterpret_cast<void*>(self->field_000c + 4);
        auto setFlag = reinterpret_cast<void (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, subObj, 0)[20]);
        setFlag(subObj, nullptr, &flag);
    }

    self->field_0018 = 1;

    void* target = reinterpret_cast<void*>(self->field_000c);
    auto request = reinterpret_cast<int (__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, target, 0)[35]);
    return request(target, nullptr, 48, 0, -1, 0);
}

HOOK(0x4E3610, C_0083e248_ResetStateToIdle, "auto");
