#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Dispatch_NoLinkedTarget: __thiscall, ret 8 (self = a2 via ecx, one stack arg).
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

// GetLinkedObjectValue: __thiscall-style, self passed in ecx, returns linked object value.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Resolves the linked object for a2 and returns its base (value - 4), or null when there is no link.
static uint8_t* ResolveLinkedObjectBase(int a2)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a2), nullptr);
    if (linkedValue)
        return reinterpret_cast<uint8_t*>(linkedValue - 4);
    return nullptr;
}

// Calls the object's slot-35 virtual method (vtable offset 0x8C) with the four given arguments.
static void InvokeLinkedObjectSlot35(uint8_t* linkedBase, int arg0, int arg1, int arg2, int arg3)
{
    void* obj = CSF_AT(void*, linkedBase, 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, obj, 0)[0x8C / 4])(
        obj, nullptr, arg0, arg1, arg2, arg3);
}

// Decrements the float timer at this+4 by a3; when it drops below zero, stops the linked
// object's current activity (slot 0x8C) and dispatches the no-linked-target path.
static void __fastcall TMovilAPunto2_ConsumeTimerStep(float* self, void* /*edx*/, int a2, float a3)
{
    InvokeLinkedObjectSlot35(ResolveLinkedObjectBase(a2), 42, 0, -1, 0);

    float remainingTime = self[1] - a3;
    self[1] = remainingTime;

    if (remainingTime < g_fZero)
    {
        InvokeLinkedObjectSlot35(ResolveLinkedObjectBase(a2), 0, 0, -1, 0);
        Dispatch_NoLinkedTarget(reinterpret_cast<void*>(a2), nullptr, 0);
    }
}

HOOK(0x004AB910, TMovilAPunto2_ConsumeTimerStep, "auto");
