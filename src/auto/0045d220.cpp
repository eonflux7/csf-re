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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_4950B0, 0x004950B0, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves the handle record referenced through the message chain, clears the
// holder's flag byte, and if the resolved object is not marked (+0x50 == 0),
// stores the result of the object's state query into the holder's flag byte.
static char __stdcall ResolveHandleAndStoreReadyState(int stateHolder, int unusedArg, int messageRef)
{
    (void)unusedArg;

    // v3 = *(*(messageRef) + 4) + 4
    auto* messageInner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, messageRef, 0));
    uint32_t handleRecordAddr = CSF_AT(uint32_t, messageInner, 4) + 4;

    // *(byte*)(*(stateHolder + 4) + 1) = 0
    auto* holderFlags = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stateHolder, 4));
    holderFlags[1] = 0;

    uint32_t returnValue = handleRecordAddr;

    auto* handleRecord = reinterpret_cast<uint8_t*>(handleRecordAddr);
    if (CSF_AT(int32_t, handleRecord, 8) >= 0)
    {
        void* resolvedObject = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleRecordAddr)));
        returnValue = reinterpret_cast<uint32_t>(resolvedObject);

        if (resolvedObject != nullptr && CSF_AT(uint32_t, resolvedObject, 0x50) == 0)
        {
            auto* holderFlagsReload = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stateHolder, 4));
            bool readyState = sub_4950B0(resolvedObject, nullptr) != 0;
            holderFlagsReload[1] = readyState ? 1 : 0;
            returnValue = readyState ? 1u : 0u;
        }
    }

    return static_cast<char>(returnValue);
}

HOOK(0x0045D220, ResolveHandleAndStoreReadyState, "auto");
