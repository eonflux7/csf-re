#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_006a8940_ResetGathering, 0x006A8A10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_CopyGatheringInfo, 0x006A8E00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetGatheringDdl_RequestGatheringDetails, 0x00798FC0, (void* self, void* edx, void* a2, int a3, int a4, int a5));
CSF_FUNC(char, __fastcall, NetGatheringDdl_RequestGatheringUpdate, 0x007990A0, (void* self, void* edx, void* arg0, char* a3, int a4, char* a2));
CSF_FUNC(char, __fastcall, NetGatheringDdl_RequestGatheringMode, 0x00799130, (void* self, void* edx, void* a2, int a3, int a4, char a5));
CSF_FUNC(bool, __fastcall, NetGatheringDdl_RequestGatheringCreation, 0x00799190, (void* self, void* edx, void* resource, void* gatheringInfo));
CSF_FUNC(void*, __fastcall, NetGatheringDdl_ConstructGatheringMonitor, 0x0079FEF0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, NetGatheringDdl_StartGatheringMonitor, 0x0079FFF0, (void* self, void* edx, int a4));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static C_0085e760* NetGatheringDdl_CreateResource()
{
    void* storage = NetZ_OperatorNew(112);
    return storage
        ? static_cast<C_0085e760*>(ConstructResource(storage, nullptr))
        : nullptr;
}

static void S_006a8940_ReleasePendingResource(S_006a8940* self)
{
    auto* resource = reinterpret_cast<C_0085e760*>(self->field_0090);
    if (resource)
    {
        resource->vSlot0(1);
        self->field_0090 = 0;
    }
}

static void S_006a8940_ResetFailedOperation(S_006a8940* self)
{
    S_006a8940_ReleasePendingResource(self);
    S_006a8940_ResetGathering(self, nullptr);
    self->field_000c = 0;
    self->field_0008 = 2;
    self->field_0334 = 0;
}

static void __fastcall S_006a8940_AdvanceGatheringSetup(S_006a8940* self, void* /*edx*/)
{
    if (self->field_0008 != 3 || self->field_000c != 2)
        return;

    auto* gathering = reinterpret_cast<C_008310b8*>(self->field_0080);
    if (!gathering)
        return;

    switch (self->field_0010)
    {
    case 0:
    {
        self->field_0010 = 1;
        NetGatheringDdl_CopyGatheringInfo(
            reinterpret_cast<uint8_t*>(self) + 0x84, nullptr,
            reinterpret_cast<int>(gathering));

        auto* resource = NetGatheringDdl_CreateResource();
        void* service = reinterpret_cast<void*>(self->field_002c);
        self->field_0090 = reinterpret_cast<uint32_t>(resource);
        if (!NetGatheringDdl_RequestGatheringCreation(
                service, nullptr, resource,
                reinterpret_cast<uint8_t*>(self) + 0x84))
        {
            self->field_02b0 = 1229729849;
            S_006a8940_ResetFailedOperation(self);
        }
        break;
    }

    case 1:
    {
        S_006a8940_ReleasePendingResource(self);
        self->field_0010 = 2;

        auto* resource = NetGatheringDdl_CreateResource();
        self->field_0090 = reinterpret_cast<uint32_t>(resource);
        CSF_AT(uint32_t, self, 0x8C) =
            reinterpret_cast<C_008310b8*>(self->field_0080)->field_0004;

        alignas(8) uint8_t temporaryStringBytes[0x4];
        InitializeTemporaryString(
            temporaryStringBytes, nullptr,
            reinterpret_cast<std::int32_t>(&g_byte_82E65C));

        const bool failed = NetGatheringDdl_RequestGatheringDetails(
            reinterpret_cast<void*>(self->field_002c), nullptr,
            reinterpret_cast<void*>(self->field_0090),
            CSF_AT(int, self, 0x8C),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xAD),
            reinterpret_cast<int>(temporaryStringBytes)) == 0;

        DestroyTemporaryString(temporaryStringBytes, nullptr);
        if (failed)
        {
            S_006a8940_ResetFailedOperation(self);
            self->field_02b0 = 1229730097;
        }
        break;
    }

    case 2:
    {
        S_006a8940_ReleasePendingResource(self);
        self->field_0010 = 3;

        auto* resource = NetGatheringDdl_CreateResource();
        char* gatheringId = CSF_AT(char*, self, 0x8C);
        void* service = reinterpret_cast<void*>(self->field_002c);
        self->field_0090 = reinterpret_cast<uint32_t>(resource);
        if (!NetGatheringDdl_RequestGatheringUpdate(
                service, nullptr, resource, gatheringId,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xAD),
                reinterpret_cast<char*>(&g_byte_82E65C)))
        {
            S_006a8940_ResetFailedOperation(self);
            self->field_02b0 = 1229730098;
        }
        break;
    }

    case 3:
    {
        S_006a8940_ReleasePendingResource(self);
        self->field_0010 = 4;

        auto* resource = NetGatheringDdl_CreateResource();
        int gatheringId = CSF_AT(int, self, 0x8C);
        void* service = reinterpret_cast<void*>(self->field_002c);
        self->field_0090 = reinterpret_cast<uint32_t>(resource);
        if (!NetGatheringDdl_RequestGatheringMode(
                service, nullptr, resource, gatheringId,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xAD),
                2))
        {
            S_006a8940_ResetFailedOperation(self);
            self->field_02b0 = 1229731641;
        }
        break;
    }

    case 4:
    {
        S_006a8940_ReleasePendingResource(self);

        void* oldMonitor = CSF_AT(void*, self, 0x330);
        if (oldMonitor)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, oldMonitor, 0)[0])(oldMonitor, nullptr, 1);
            CSF_AT(void*, self, 0x330) = nullptr;
        }

        void* storage = NetZ_OperatorNew(32);
        void* monitor = storage
            ? NetGatheringDdl_ConstructGatheringMonitor(
                  storage, nullptr, static_cast<int>(self->field_002c), 600)
            : nullptr;

        int gatheringId = CSF_AT(int, self, 0x8C);
        CSF_AT(void*, self, 0x330) = monitor;
        NetGatheringDdl_StartGatheringMonitor(monitor, nullptr, gatheringId);
        self->field_0008 = 7;
        self->field_000c = 0;
        break;
    }

    default:
        return;
    }
}

HOOK(0x006A9270, S_006a8940_AdvanceGatheringSetup, "auto");
