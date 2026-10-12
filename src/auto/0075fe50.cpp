#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_69DC30, 0x0069DC30, (void* self, void* edx, char* a1, int a2));

// Calls sub_69DC30 with this+8 as the register and second stack argument.
// If the 0x12C field is below 16, the address of the 0x118 field is passed as the first stack argument;
// otherwise the value of the 0x118 field is passed.
static char __fastcall C_0086c2b4_CallWithFieldAddressOrValue(C_0086c2b4* self, void* /*edx*/)
{
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);
    void* selfPlus8 = selfBytes + 8;
    int selfPlus8Value = static_cast<int>(reinterpret_cast<std::uintptr_t>(selfPlus8));

    if (self->field_012c < 0x10)
    {
        char* fieldAddress = reinterpret_cast<char*>(&self->field_0118);
        return sub_69DC30(static_cast<void*>(self), selfPlus8, fieldAddress, selfPlus8Value);
    }
    else
    {
        char* fieldValue = reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0118));
        return sub_69DC30(static_cast<void*>(self), selfPlus8, fieldValue, selfPlus8Value);
    }
}

HOOK(0x75FE50, C_0086c2b4_CallWithFieldAddressOrValue, "auto");
