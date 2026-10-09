#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_482D30, 0x00482D30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GuiControl_SetInteractionState, 0x0048EEC0, (void* self, void* edx, int a2));

static int __fastcall CCharActionFsm_DispatchInteraction(CCharActionFsm* self, void* /*edx*/)
{
    auto* target = reinterpret_cast<C_00833ee8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(&self->field_0040)));

    int result = sub_482D30(self, nullptr, reinterpret_cast<int>(target));
    if (!target)
        return result;

    int actionType = 0;
    if (self->field_0024 != static_cast<uint32_t>(-1))
    {
        void* action = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero_37());
        actionType = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, action, 0)[0x50 / 4])(action, nullptr);
    }

    result = static_cast<int>(static_cast<uint32_t>(actionType) - 5);

    int requestType;
    switch (actionType)
    {
    case 5:
        requestType = 9;
        break;
    case 6:
        requestType = 10;
        break;
    case 7:
        return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
            target->vtbl[0x8C / 4])(target, nullptr, 34, 0, -1, 0);
    case 8:
        requestType = 15;
        break;
    default:
        return result;
    }

    GuiControl_SetInteractionState(target, nullptr, 1);

    uint32_t request[0x30 / sizeof(uint32_t)];
    request[11] = requestType;
    request[1] = static_cast<uint32_t>(-1);
    request[2] = static_cast<uint32_t>(-1);
    request[3] = static_cast<uint32_t>(-1);

    auto* ownerHandle = reinterpret_cast<uint8_t*>(self->field_0008) + 0x1C;
    request[4] = 0;
    request[5] = 0;
    request[9] = 0;
    request[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    request[6] = 0;
    request[7] = 0;
    request[8] = 0;
    request[10] = 0;

    request[1] = CSF_AT(uint32_t, ownerHandle, 0x4);
    request[2] = CSF_AT(uint32_t, ownerHandle, 0x8);
    void* controller = reinterpret_cast<void*>(target->field_0038);
    request[3] = request[11];
    request[4] = 0x7FFFFFFF;
    request[5] = 0;
    request[9] = 1;

    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, controller, 0)[0x8C / 4])(controller, nullptr, request);
}

HOOK(0x004855C0, CCharActionFsm_DispatchInteraction, "auto");
