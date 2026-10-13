#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: edx = this->field_0004 is used directly as the function table
// (call [edx+0x34]); ecx = &this->field_0004, one stack arg = &this->field_0008.
static int __fastcall C_0083aa80_ForwardSlot34ToHeldObject(C_0083aa80* self, void* /*edx*/)
{
    void* table = reinterpret_cast<void*>(self->field_0004);
    auto slot34 = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(CSF_AT(void*, table, 0x34));
    return slot34(&self->field_0004, nullptr, reinterpret_cast<uint8_t*>(self) + 0x08);
}

HOOK(0x004AA300, C_0083aa80_ForwardSlot34ToHeldObject, "auto");
