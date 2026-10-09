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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneFallbackLookup, 0x004686A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneFindStateRecord, 0x004686F0, (void* self, void* edx, int a2, int a3));

// Integer handle value -> pointer (32-bit target).
static void* AsPointer(int value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

// Reads the lookup key: *(*(a3[1] + 4) + 4).
static int ReadLookupKey(void** requestRecord)
{
    void* keyHolder = requestRecord[1];
    void* keyRecord = CSF_AT(void*, keyHolder, 4);
    return CSF_AT(int, keyRecord, 4);
}

static int __stdcall ResolveHandleStateByteOrFallback(void* stateHolder, void* /*unusedArg*/, void** requestRecord)
{
    // Clear the state byte of the object referenced by stateHolder->+4.
    static_cast<uint8_t*>(CSF_AT(void*, stateHolder, 4))[1] = 0;

    void* request = requestRecord[0];
    void* handleRecord = CSF_AT(void*, request, 4);
    int handleRecordTag = CSF_AT(int, handleRecord, 12);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(handleRecord) + 4);

    if (handleRecordTag >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            int lookupKey = ReadLookupKey(requestRecord);
            result = SceneFindStateRecord(CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr, result, lookupKey);
            if (result)
            {
                uint8_t stateByte = CSF_AT(uint8_t, AsPointer(result), 128);
                static_cast<uint8_t*>(CSF_AT(void*, stateHolder, 4))[1] = stateByte;
                return result;
            }
            int fallbackKey = ReadLookupKey(requestRecord);
            return SceneFallbackLookup(CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr, fallbackKey);
        }
    }
    return result;
}

[[maybe_unused]] static const auto kFallbackLookupVtable = CSF_VTBL(0x004686A0);

HOOK(0x0045D300, ResolveHandleStateByteOrFallback, "auto");
