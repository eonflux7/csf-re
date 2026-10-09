#include <cstdint>
#include "globals.h"
#include "hook.h"

// Binds to the original original callees by address.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));

// Stores the modulo of a global state value into a float field.
// Returns the quotient when the divisor is positive; otherwise zeroes the field and returns the divisor.
static int __stdcall ComputeGlobalStateModulo(uint32_t a1, uint32_t a2, uint32_t* a3) {
    (void)a2;

    // Read the float divisor: *(*(*a3) + 4) -> object; its +4 holds the float.
    void* sourceObject = reinterpret_cast<void*>(*a3);
    void* sourceInner = CSF_AT(void*, sourceObject, 4);
    float divisorFloat = CSF_AT(float, sourceInner, 4);

    int64_t divisorWide = static_cast<int64_t>(divisorFloat);
    int divisor = static_cast<int>(divisorWide);

    if (divisor <= 0) {
        void* outputHolder = CSF_AT(void*, reinterpret_cast<void*>(a1), 4);
        CSF_AT(uint32_t, outputHolder, 4) = 0;
        return divisor;
    }

    void* targetObject = CSF_AT(void*, reinterpret_cast<void*>(a1), 4);

    int globalState = GetGlobalState_50D190();
    int stateValue = GlobalStateValue_50DCF0(reinterpret_cast<void*>(globalState), nullptr);

    int remainder = stateValue % divisor;
    CSF_AT(float, targetObject, 4) = static_cast<float>(remainder);

    return stateValue / divisor;
}

HOOK(0x0045C3E0, ComputeGlobalStateModulo, "auto");
