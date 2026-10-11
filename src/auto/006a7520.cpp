#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetGatheringDdl_InitializeResource, 0x00798FF0, (void* self, void* edx, void* a2, int a3, int a4, int a5));
CSF_FUNC(void, __fastcall, NetGatheringDdl_StartOperation, 0x007A1240, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_ConstructOperation, 0x007A18E0, (void* self, void* edx, int a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void __fastcall S_006a7520_UpdateResourceState(
    S_006a7520* self, void* /*edx*/, int /*unused*/)
{
    void* resource = reinterpret_cast<void*>(self->field_0090);
    const uint32_t resourceType = CSF_AT(uint32_t, resource, 0x0C);
    if (resourceType == 1)
        return;

    if (resource)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
        self->field_0090 = 0;
    }

    if (resourceType == 2)
    {
        void* storage = NetZ_OperatorNew(16);
        uint32_t* operation = nullptr;
        if (storage)
        {
            operation = NetGatheringDdl_ConstructOperation(
                storage, nullptr, static_cast<int>(self->field_0038));
        }

        self->field_0320 = reinterpret_cast<uint32_t>(operation);
        NetGatheringDdl_StartOperation(
            operation, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x60));
        self->field_0018 = 3;
        self->field_0324 = 1084227584;
        return;
    }

    if (resourceType == 3)
        self->field_02b0 = 1229730352;
    else
        self->field_02b0 = 1229730357;

    void* storage = NetZ_OperatorNew(112);
    void* replacement = nullptr;
    if (storage)
        replacement = ConstructResource(storage, nullptr);
    self->field_0090 = reinterpret_cast<uint32_t>(replacement);

    alignas(8) uint8_t temporaryStringBytes[0x4];
    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(&g_byte_82E65C));

    NetGatheringDdl_InitializeResource(
        CSF_AT(void*, self, 0x2C), nullptr,
        reinterpret_cast<void*>(self->field_0090),
        CSF_AT(int, self, 0x8C),
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x2B4),
        reinterpret_cast<int>(temporaryStringBytes));

    DestroyTemporaryString(temporaryStringBytes, nullptr);
    self->field_0018 = 5;
}

HOOK(0x006A7520, S_006a7520_UpdateResourceState, "auto");
