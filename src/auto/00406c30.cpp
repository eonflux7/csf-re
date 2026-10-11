#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// Vtable slot signatures used by the original (slot = byte offset / 4).
using GetStateObjectFn = void* (__fastcall*)(void* self, void* edx);        // slot 0x28 of field_0008
using GetSubStateFn = void* (__fastcall*)(void* self, void* edx);           // slot 0x68 of state object
using QueryActiveFlagFn = int (__fastcall*)(void* self, void* edx);         // slot 0x48 of sub-state
using SetEntryTargetFn = void (__fastcall*)(void* self, void* edx, int entryId, float* vec, int extra); // slot 0x78

static int __fastcall C_0082d77c_StopAllActiveEntries(C_0082d77c* self, void* /*edx*/)
{
    // Resolve the controller chain: field_0008 -> state object -> sub-state.
    void* controller = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));
    void* stateObject = (CSF_AT(GetStateObjectFn*, controller, 0)[0x28 / 4]) == nullptr
                            ? nullptr
                            : reinterpret_cast<GetStateObjectFn>(CSF_AT(void**, controller, 0)[0x28 / 4])(controller, nullptr);
    void* subState = reinterpret_cast<GetSubStateFn>(CSF_AT(void**, stateObject, 0)[0x68 / 4])(stateObject, nullptr);

    int result = reinterpret_cast<QueryActiveFlagFn>(CSF_AT(void**, subState, 0)[0x48 / 4])(subState, nullptr);
    if (static_cast<uint8_t>(result) != 0)
    {
        for (unsigned int i = 0; i < 0x84; i += 44)
        {
            uint32_t tableBase = self->field_000c;
            uint8_t activeFlag = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(tableBase))[i];
            result = static_cast<int>(i + tableBase);
            if (activeFlag != 0)
            {
                int entryId = CSF_AT(int, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(tableBase)), i + 8);
                if (entryId != -1)
                {
                    reinterpret_cast<SetEntryTargetFn>(CSF_AT(void**, subState, 0)[0x78 / 4])(
                        subState, nullptr, entryId, &g_ZeroVectorX, 0);
                }
                result = static_cast<int>(self->field_000c);
                reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c))[i] = 0;
            }
        }
    }
    return result;
}

HOOK(0x00406C30, C_0082d77c_StopAllActiveEntries, "auto");
