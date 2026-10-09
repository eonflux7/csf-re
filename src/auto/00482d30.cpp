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

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ObjectProvider_SetPendingRequest, 0x004F5EF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(void, __fastcall, ChannelController_Reset, 0x0049AF10, (void* self, void* edx, char a3));
CSF_FUNC(int, __fastcall, ChannelController_Configure, 0x00498F50, (void* self, void* edx, int a2, char a3));

static int __fastcall CCharActionFsm_ResetActionProviderState(CCharActionFsm* self, void* /*edx*/, int ownerRef)
{
    // Optional pre-step: if the owner's inner object reports a state, notify it.
    if (ownerRef) {
        uint8_t* ownerBase = reinterpret_cast<uint8_t*>(ownerRef);
        void* innerObject = CSF_AT(void*, ownerBase, 0x44);

        using SlotD8Fn = int(__fastcall*)(void*, void*);
        using SetFlagFn = void(__fastcall*)(void*, void*, int);

        SlotD8Fn queryInner = reinterpret_cast<SlotD8Fn>(CSF_AT(void**, innerObject, 0)[0xD8 / 4]);
        if (queryInner(innerObject, nullptr)) {
            void* innerChild = reinterpret_cast<void*>(queryInner(innerObject, nullptr));
            SetFlagFn setFlag = reinterpret_cast<SetFlagFn>(CSF_AT(void**, innerChild, 0)[0x30 / 4]);
            setFlag(innerChild, nullptr, 1);
        }
    }

    // The pending-request argument is computed from the channel controller field before any calls.
    int pendingRequestArg = static_cast<int>(self->field_0008 + 0x1c);

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    ObjectProvider_SetPendingRequest(provider, nullptr, pendingRequestArg);

    ChannelController_Reset(reinterpret_cast<void*>(self->field_0008), nullptr, 0);
    ChannelController_Configure(reinterpret_cast<void*>(self->field_0008), nullptr, 4096, 0);

    int globalValueAgain = GetGlobalValue_4FBDE0();
    int firstElementAgain = GetFirstElement(reinterpret_cast<void*>(globalValueAgain), nullptr);
    void* providerAgain = GetCachedObjectProvider(reinterpret_cast<void*>(firstElementAgain), nullptr);
    return ObjectProvider_SetUpdateMode(providerAgain, nullptr, 0);
}

HOOK(0x00482D30, CCharActionFsm_ResetActionProviderState, "auto");
