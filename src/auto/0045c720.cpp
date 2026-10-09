#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(double, __stdcall, ConvertValueToScalar, 0x00523140, (int a1));

// Reads the dword at base+off as a pointer.
static void* ReadPtrAt(void* base, uint32_t off) {
    return reinterpret_cast<void*>(*CSF_AT(uint32_t*, base, off));
}

// Clears the dword at a1->field_0004->field_0004, then, unless the source value
// (a3 -> ptr -> ptr -> dword at +4) is -1, converts that value through
// ConvertValueToScalar and stores the result as a float in the same slot
// (re-reading a1->field_0004 after the calls).
static void __stdcall ResetAndRefreshScalar(void* a1, void* a2, void* a3) {
    void* outputHolder = ReadPtrAt(a1, 4);
    *CSF_AT(uint32_t*, outputHolder, 4) = 0;

    void* sourceLevel1 = ReadPtrAt(a3, 0);
    void* sourceLevel2 = ReadPtrAt(sourceLevel1, 4);
    uint32_t sourceValue = *CSF_AT(uint32_t*, sourceLevel2, 4);

    if (sourceValue != 0xFFFFFFFFu) {
        int value = static_cast<int>(sourceValue);
        GetGlobalValue_521BB0();
        double scalar = ConvertValueToScalar(value);
        void* outputHolderAfterCalls = ReadPtrAt(a1, 4);
        *CSF_AT(float*, outputHolderAfterCalls, 4) = static_cast<float>(scalar);
    }
}

HOOK(0x0045C720, ResetAndRefreshScalar, "auto");
