#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_DestroyMember50, 0x006508A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_DestroyMember60, 0x00652810, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_DestroyMember70, 0x006A6FB0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_DestroyMember40, 0x006A7000, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ClearGatheringState, 0x006A9A60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ClearAdditionalState, 0x006AA6F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_DestroyOwnedObject28, 0x007912F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetGatheringDdl_DestroyOwnedObject3C, 0x007A06A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_DestroyOwnedObject320, 0x007A14C0, (void* self, void* edx));

static void __fastcall C_0085fe28_ReleaseResources(C_0085fe28* self, void* /*edx*/)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        self->vtbl[0x44 / 4])(self, nullptr, 1);

    NetGatheringDdl_ClearGatheringState(self, nullptr);
    NetGatheringDdl_ClearAdditionalState(self, nullptr);

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    NetGatheringDdl_DestroyMember40(bytes + 0x40, nullptr);
    NetGatheringDdl_DestroyMember50(bytes + 0x50, nullptr);
    NetGatheringDdl_DestroyMember60(bytes + 0x60, nullptr);
    NetGatheringDdl_DestroyMember70(bytes + 0x70, nullptr);

    uint32_t storage = self->field_0320;
    if (storage)
    {
        NetGatheringDdl_DestroyOwnedObject320(reinterpret_cast<void*>(storage), nullptr);
        NetSession_ReleaseTransportStorage(static_cast<int>(storage));
        self->field_0320 = 0;
    }

    void* object = reinterpret_cast<void*>(self->field_0330);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0330 = 0;
    }

    storage = self->field_003c;
    if (storage)
    {
        NetGatheringDdl_DestroyOwnedObject3C(reinterpret_cast<void*>(storage), nullptr);
        NetSession_ReleaseTransportStorage(static_cast<int>(storage));
        self->field_003c = 0;
    }

    object = reinterpret_cast<void*>(self->field_0034);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0034 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0030);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0030 = 0;
    }

    object = reinterpret_cast<void*>(self->field_002c);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_002c = 0;
    }

    storage = self->field_0028;
    if (storage)
    {
        NetGatheringDdl_DestroyOwnedObject28(reinterpret_cast<void*>(storage), nullptr);
        NetSession_ReleaseTransportStorage(static_cast<int>(storage));
        self->field_0028 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0090);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0090 = 0;
    }
}

HOOK(0x006AA760, C_0085fe28_ReleaseResources, "auto");
