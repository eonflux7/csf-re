#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Calls the object's vtbl slot 0x50 (no args), vtbl slot 0x14 (one int arg).

static uint32_t* __fastcall C_0082e090_ResolveLinkedChild(C_0082e090* self, void* /*edx*/) {
    uint32_t* result = reinterpret_cast<uint32_t*>(self->field_0004);
    if (CSF_AT(int, result, 0x50) == 0x15) {
        // result->vtbl[0x28 / 4]()
        result = reinterpret_cast<uint32_t*>(
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, result, 0)[0x28 / 4])(result, nullptr));
        if (result) {
            // child = result->vtbl[0x50 / 4]()
            uint32_t* child = reinterpret_cast<uint32_t*>(
                ((void* (__fastcall*)(void*, void*))CSF_AT(void**, result, 0)[0x50 / 4])(result, nullptr));
            // return child->vtbl[0x14 / 4](0x40000000)
            return reinterpret_cast<uint32_t*>(
                ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0x14 / 4])(
                    child, nullptr, 0x40000000));
        }
    }
    return result;
}

HOOK(0x00416220, C_0082e090_ResolveLinkedChild, "auto");
