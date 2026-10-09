#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(double, __stdcall, ConvertValueToScalar, 0x00523140, (int a1));

// Zeroes the scalar slot of the context's target object, then, when the
// referenced source value is not -1, refreshes it from the global value table.
static void __stdcall ResetAndRefreshScalar(int a1, int a2, int a3)
{
    (void)a2;

    // Context -> target object pointer at +4.
    uint32_t targetObject = CSF_AT(uint32_t, a1, 4);
    CSF_AT(int, targetObject, 4) = 0;

    // Source chain: a3 -> *a3 (node) -> node+4 (record) -> record+4 (value).
    uint32_t sourceNode = CSF_AT(uint32_t, a3, 0);
    uint32_t sourceRecord = CSF_AT(uint32_t, sourceNode, 4);
    int sourceValue = CSF_AT(int, sourceRecord, 4);

    if (sourceValue != -1)
    {
        int valueArg = sourceValue;
        [[maybe_unused]] int globalValue = GetGlobalValue_521BB0();
        float scalar = static_cast<float>(ConvertValueToScalar(valueArg));

        // Target pointer is reloaded after the calls, as in the original.
        uint32_t targetAfterCalls = CSF_AT(uint32_t, a1, 4);
        CSF_AT(float, targetAfterCalls, 4) = scalar;
    }
}

HOOK(0x0045C720, ResetAndRefreshScalar, "auto");
