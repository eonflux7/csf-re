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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, (void));
CSF_FUNC(int, __fastcall, GlobalStateSlot_ReadActiveId, 0x0050E1C0, (void* self, void* edx));

// Calls the virtual handler at vtable slot 0x48 (slot 18) with the message pointer.
static int ForwardToVirtualMessageHandler(CMenuMpOnline* self, uint32_t* message)
{
    using VirtualMessageHandler = int(__fastcall*)(void* self, void* edx, uint32_t* message);
    auto handler = reinterpret_cast<VirtualMessageHandler>(CSF_AT(void**, self, 0)[18]);
    return handler(self, nullptr, message);
}

static int __fastcall CMenuMpOnline_HandleMessage(CMenuMpOnline* self, void* /*edx*/, uint32_t* message)
{
    const uint32_t messageId = message[0];

    if (messageId == 13)
    {
        const uint32_t sourceId = message[1];

        if (sourceId == self->field_001c)
        {
            self->field_0034 = 1;
            return ForwardToVirtualMessageHandler(self, message);
        }
        if (sourceId == self->field_0020)
        {
            self->field_0035 = 1;
            return ForwardToVirtualMessageHandler(self, message);
        }
        if (sourceId == self->field_0024)
        {
            self->field_0036 = 1;
            return ForwardToVirtualMessageHandler(self, message);
        }
        if (sourceId == self->field_0028)
        {
            self->field_0037 = 1;
        }
        return ForwardToVirtualMessageHandler(self, message);
    }

    if (messageId == 14)
    {
        const uint32_t sourceId = message[1];
        const int globalStateValue = GetGlobalState_50D190();
        void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(globalStateValue));
        const int resolvedId = GlobalStateSlot_ReadActiveId(globalState, nullptr);

        if (sourceId == static_cast<uint32_t>(resolvedId) && self->field_0018 == 1)
        {
            self->field_0018 = 2;
        }
        return resolvedId;
    }

    // Other message ids: returns (messageId - 14) with 32-bit wraparound, as the original does.
    return static_cast<int>(messageId - 14u);
}

HOOK(0x0055F680, CMenuMpOnline_HandleMessage, "auto");
