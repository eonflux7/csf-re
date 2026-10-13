#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees (bound by address).
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(int, __fastcall, sub_69B7B0, 0x0069B7B0, (void* self, void* edx, unsigned int a2, unsigned int arg4, uint8_t* a3));

// Returns the bucket lookup result when (a1 & a3) is nonzero and the lookup reports success.
static uint32_t CoreParser_LookupMaskedValue(hook::Regs* regs, const uint32_t* args) {
    (void)args;
    const uint32_t a1 = regs->eax;
    const uint32_t a2 = regs->edx;
    const uint32_t a3 = regs->ecx;

    if ((a3 & a1) == 0) {
        return 0;
    }

    uint8_t lookupSucceeded = 0;
    int32_t globalValue = GetGlobalValue_69B700();
    void* bucketSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(globalValue)));
    uint32_t result = static_cast<uint32_t>(sub_69B7B0(bucketSelf, nullptr, a2, a1, &lookupSucceeded));

    if (!lookupSucceeded) {
        return 0;
    }
    return result;
}

HOOK_REGS(0x692410, CoreParser_LookupMaskedValue, 0, "core_parser");
