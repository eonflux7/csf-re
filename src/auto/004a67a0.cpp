#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Reads the object stored at self+0x08, calls its vtable slot 10 (+0x28) to get
// a sub-object, calls that object's slot 26 (+0x68) to get another object, then
// calls slot 7 (+0x1C) on it with arguments (20, 0) and returns the result.
static int __fastcall QueryNestedChainValue20(void* self, void* /*edx*/) {
    void* outerObject = CSF_AT(void*, self, 0x08);

    void* middleObject = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))(CSF_AT(void**, outerObject, 0)[0x28 / 4]))(outerObject, nullptr));

    void* innerObject = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))(CSF_AT(void**, middleObject, 0)[0x68 / 4]))(middleObject, nullptr));

    return ((int(__fastcall*)(void*, void*, int, int))(CSF_AT(void**, innerObject, 0)[0x1C / 4]))(innerObject, nullptr, 20, 0);
}

HOOK(0x004A67A0, QueryNestedChainValue20, "auto");
