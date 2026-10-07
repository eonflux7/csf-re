#include "globals.h"
#include "hook.h"

// 0x006D59F0  double __thiscall sub_6D59F0(float *this)
CSF_FUNC(double, __fastcall, C_00864508_GetField9c, 0x6D59F0, (float* self, void* edx));

// 0x006CAF40:  mov ecx, [ecx+0xBC] ; jmp 0x6D59F0
// Loads the object pointer stored at this+0xBC and tail-forwards it to
// C_00864508_GetField9c, returning that call's double result unchanged.
static double __fastcall GetNestedObjectField9c(void* self, void* /*edx*/)
{
    float* nestedObject = CSF_AT(float*, self, 0xBC);
    return C_00864508_GetField9c(nestedObject, nullptr);
}

HOOK(0x6CAF40, GetNestedObjectField9c, "auto");
