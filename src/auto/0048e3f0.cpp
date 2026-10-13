#include <cstdint>
#include "globals.h"
#include "hook.h"

// Resolves the object at self+0x16C, passes resolveArg through its slot 34 to get a
// sub-object, then, if the slot-32 lookup on the same object returns non-null, forwards
// valueArg to that result's slot 24.
static int __fastcall ApplyScaledValueToResolvedObject(void* self, void* /*edx*/, int resolveArg, int valueArg)
{
    using ResolveFn = void* (__fastcall*)(void* self, void* edx, int arg);
    using LookupFn = void* (__fastcall*)(void* self, void* edx, void* arg);
    using ApplyFn = int (__fastcall*)(void* self, void* edx, int arg);

    // v4 = (*this[0x16C])->vtbl[0x88 / 4](this[0x16C], resolveArg)
    void* baseObj = CSF_AT(void*, self, 0x16C);
    void* resolvedKey = reinterpret_cast<ResolveFn>(CSF_AT(void**, baseObj, 0)[0x88 / 4])(
        baseObj, nullptr, resolveArg);

    // result = (*this[0x16C])->vtbl[0x80 / 4](this[0x16C], v4)
    baseObj = CSF_AT(void*, self, 0x16C);
    void* result = reinterpret_cast<LookupFn>(CSF_AT(void**, baseObj, 0)[0x80 / 4])(
        baseObj, nullptr, resolvedKey);

    if (result == nullptr)
        return 0;

    // return result->vtbl[0x60 / 4](result, valueArg)
    return reinterpret_cast<ApplyFn>(CSF_AT(void**, result, 0)[0x60 / 4])(result, nullptr, valueArg);
}

HOOK(0x0048E3F0, ApplyScaledValueToResolvedObject, "auto");
