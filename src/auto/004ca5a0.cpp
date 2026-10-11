#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
// Actually a __thiscall method (ecx = this, one stack arg).
CSF_FUNC(int, __fastcall, TaskSetup_4C95B0, 0x004C95B0, (void* self, void* edx, int a1));

// Returns the object that contains the linked sub-object (value - 4), or null.
static uint8_t* GetOwnerObject(int link)
{
    int value = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(link), nullptr);
    if (value)
        return reinterpret_cast<uint8_t*>(value - 4);
    return nullptr;
}

static int __fastcall CoverAttack_ResetOwnerState(void* self, void* /*edx*/, int link)
{
    uint8_t* obj;

    obj = GetOwnerObject(link);
    CSF_AT(int, obj, 0x6DC) = 0;

    obj = GetOwnerObject(link);
    C_004D10F0_ResetFields(obj + 0x690, nullptr);

    obj = GetOwnerObject(link);
    {
        void* sub = CSF_AT(void*, obj, 0x2C);
        void** vtbl = CSF_AT(void**, sub, 0);
        ((void(__fastcall*)(void*, void*, int, int, int, int))vtbl[0x8C / 4])(sub, nullptr, 0, 0, -1, 0);
    }

    TaskSetup_4C95B0(self, nullptr, link);

    obj = GetOwnerObject(link);
    CSF_AT(int, obj, 0x3B8) = 1;
    CSF_AT(int, obj, 0x3C0) = 0;
    CSF_AT(int, obj, 0x3C4) = 0;
    CSF_AT(float, obj, 0x3C0) = 0.1f;
    CSF_AT(float, obj, 0x3C4) = 0.1f;

    obj = GetOwnerObject(link);
    {
        uint8_t* sub = obj + 0x258;
        void** vtbl = CSF_AT(void**, obj, 0x258);
        ((void(__fastcall*)(void*, void*, int))vtbl[0x28 / 4])(sub, nullptr, link);
    }

    uint8_t* src = GetOwnerObject(link);
    uint8_t* dst = GetOwnerObject(link);
    CSF_AT(uint8_t, dst, 0x26C) = CSF_AT(uint8_t, src, 0x6B0);

    obj = GetOwnerObject(link);
    CSF_AT(float, obj, 0x264) = 0.3f;
    CSF_AT(float, obj, 0x268) = 0.3f;

    int value = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(link), nullptr);
    if (value) {
        CSF_AT(uint8_t, value, 0x6AC) = 0;
        return value;
    }
    CSF_AT(uint8_t, static_cast<uint8_t*>(nullptr), 0x6B0) = 0;
    return 0;
}

HOOK(0x004CA5A0, CoverAttack_ResetOwnerState, "auto");
