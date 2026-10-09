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

CSF_GLOBAL(int32_t, g_Value_8B0674, 0x008B0674);

CSF_FUNC(int, __fastcall, DispatchGlobalValueMessage, 0x00522330, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_523FA0, 0x00523FA0, (void* self, void* edx, uint32_t* a2, int a3));

// Forwards a sound request to the global value dispatcher. If the dispatcher
// returns a non-negative slot index, the request's value is stored into that
// slot of the table at this+0x10. Otherwise a 6-dword request record (with the
// C_0082d448 vtable tag) is built on the stack and queued via sub_523FA0 on
// the sub-object at this+0x10.
static int __fastcall SoundManager_DispatchOrQueueRequest(S_00523660* self, void* /*edx*/, int arg0, int a3)
{
    int dispatchResult = DispatchGlobalValueMessage(reinterpret_cast<void*>(g_Value_8B0674), nullptr, arg0);

    if (dispatchResult < 0)
    {
        uint32_t queuedArgs[6];
        void* requestSource = reinterpret_cast<void*>(arg0);

        queuedArgs[3] = CSF_AT(uint32_t, requestSource, 0x04);

        float positionZ = CSF_AT(float, requestSource, 0x0C);
        std::memcpy(&queuedArgs[5], &positionZ, sizeof(float));

        queuedArgs[0] = CSF_AT(uint32_t, reinterpret_cast<void*>(g_Value_8B0674), 0x5C);
        queuedArgs[4] = CSF_AT(uint32_t, requestSource, 0x08);
        queuedArgs[1] = static_cast<uint32_t>(a3);
        queuedArgs[2] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));

        uint32_t* queuedResult = sub_523FA0(
            reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x10),
            nullptr,
            queuedArgs,
            0);
        return static_cast<int>(reinterpret_cast<uintptr_t>(queuedResult));
    }

    int result = 3 * dispatchResult;
    void* slotTable = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    CSF_AT(int32_t, slotTable, 8 * result + 4) = a3;
    return result;
}

HOOK(0x00524230, SoundManager_DispatchOrQueueRequest, "auto");
