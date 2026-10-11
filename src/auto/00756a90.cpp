#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_69DC30, 0x0069DC30, (void* self, void* edx, char* a1, int a2));

// Calls sub_69DC30 with this+8 as the second register and stack argument.
// If the 0x22A0 field is below 16, the 0x228C field's address is passed; otherwise its value is passed.
static char __fastcall C_0086a3d8_CallWithStringField(C_0086a3d8* self, void* /*edx*/)
{
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);
    void* selfPlus8 = selfBytes + 8;
    int selfPlus8Value = static_cast<int>(reinterpret_cast<std::uintptr_t>(selfPlus8));

    if (self->field_22a0 < 0x10)
    {
        char* fieldAddress = reinterpret_cast<char*>(&self->field_228c);
        return sub_69DC30(static_cast<void*>(self), selfPlus8, fieldAddress, selfPlus8Value);
    }
    else
    {
        char* fieldPointer = reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_228c));
        return sub_69DC30(static_cast<void*>(self), selfPlus8, fieldPointer, selfPlus8Value);
    }
}

HOOK(0x756A90, C_0086a3d8_CallWithStringField, "auto");
