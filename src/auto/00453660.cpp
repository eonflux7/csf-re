#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x004511A0: receives the caller's S_0041e5f0 and float/int pointers plus this object's
// sub-field address; __thiscall with 5 stack args (ret 20).
CSF_FUNC(int, __fastcall, CGuiControl_PropagateFlagsToChild, 0x004511A0,
         (void* self, void* edx, void* a1, float* a2, int* a3, uint32_t* a4, float a5));

// 0x00451250: __thiscall with 4 stack args (ret 16).
CSF_FUNC(int, __fastcall, CGuiControl_ComputeDispatchResult, 0x00451250,
         (void* self, void* edx, uint32_t* a2, int a3, float a4, int a5));

static int __fastcall CGuiControl_ProcessDisplayFlags(CGuiControl* self, void* /*edx*/,
                                                      S_0041e5f0* a2, float* a3, int* a4, float a5)
{
    int flagsBefore = *a4;
    int dispatchValue = 0;

    if ((flagsBefore & 0x20) == 0)
    {
        if ((flagsBefore & 0x10) != 0)
        {
            uint32_t* subField = reinterpret_cast<uint32_t*>((uint8_t*)self + 0x20);
            CGuiControl_PropagateFlagsToChild(self, nullptr, a2, a3, a4, subField, a5);
        }
    }
    else
    {
        CGuiControl_PropagateFlagsToChild(self, nullptr, a2, a3, a4, &self->field_0010, a5);
    }

    // Flags are re-read after the callee, which may have modified *a4.
    int result = *a4;
    if ((*a4 & 0x2000) == 0)
    {
        return CGuiControl_ComputeDispatchResult(self, nullptr,
                                                 reinterpret_cast<uint32_t*>(a2),
                                                 reinterpret_cast<int>(a3),
                                                 a5,
                                                 reinterpret_cast<int>(&dispatchValue));
    }
    return result;
}

HOOK(0x453660, CGuiControl_ProcessDisplayFlags, "auto");
