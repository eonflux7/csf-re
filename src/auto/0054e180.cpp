#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Calls a virtual method on the object held in field_0004. Passes a char
// (widened to int) when the flag byte at a3+0x14 is set, otherwise passes the
// char* itself. Returns 0 when the call's byte result is nonzero, -1 otherwise.
static int __fastcall C_008443c8_MatchNameOrNegOne(C_008443c8* self, void* /*edx*/, char* a2, int a3) {
    void* object = reinterpret_cast<void*>(self->field_0004);
    uint8_t useCharValue = CSF_AT(uint8_t, reinterpret_cast<void*>(a3), 0x14);

    if (useCharValue) {
        using CharMethodFn = uint8_t(__fastcall*)(void*, void*, int);
        CharMethodFn method = reinterpret_cast<CharMethodFn>(CSF_AT(void**, object, 0)[0x48 / 4]);
        uint8_t matched = method(object, nullptr, static_cast<int>(*a2));
        return (matched != 0) ? 0 : -1;
    }

    using StringMethodFn = uint8_t(__fastcall*)(void*, void*, char*);
    StringMethodFn method = reinterpret_cast<StringMethodFn>(CSF_AT(void**, object, 0)[0x28 / 4]);
    uint8_t matched = method(object, nullptr, a2);
    return (matched != 0) ? 0 : -1;
}

HOOK(0x0054E180, C_008443c8_MatchNameOrNegOne, "auto");
