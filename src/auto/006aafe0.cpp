#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_6A7520, 0x006A7520, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_6AAD80, 0x006AAD80, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_652810, 0x00652810, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_799070, 0x00799070, (void* self, void* edx, void* a2, int a3, int a4));

static void NetGatheringDdl_ReleaseResource(void* resource)
{
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
}

static void NetGatheringDdl_HandlePhaseOne(S_006a7520* self)
{
    void* resource = reinterpret_cast<void*>(self->field_0090);
    uint32_t resourceState = CSF_AT(uint32_t, resource, 0xC);

    if (resourceState == 1)
        return;

    if (resourceState == 2)
    {
        if (resource)
        {
            NetGatheringDdl_ReleaseResource(resource);
            self->field_0090 = 0;
        }

        if (CSF_AT(uint8_t, self, 0x2B4))
        {
            uint8_t* request = reinterpret_cast<uint8_t*>(self) + 0x60;
            sub_652810(request, nullptr);

            void* memory = NetZ_OperatorNew(112);
            void* newResource = memory ? ConstructResource(memory, nullptr) : nullptr;

            int requestValue = CSF_AT(int, self, 0x8C);
            void* owner = CSF_AT(void*, self, 0x2C);
            self->field_0090 =
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(newResource));

            if (sub_799070(owner, nullptr, newResource, requestValue,
                           static_cast<int>(reinterpret_cast<uintptr_t>(request))))
            {
                self->field_0018 = 2;
                return;
            }
            self->field_02b0 = 1229730105;
        }
        else
        {
            self->field_02b0 = 1229730358;
        }
    }
    else
    {
        self->field_02b0 =
            resourceState == 3 ? 1229730352 : 1229730356;
        if (resource)
        {
            NetGatheringDdl_ReleaseResource(resource);
            self->field_0090 = 0;
        }
    }

    self->field_000c = 0;
    self->field_0334 = 0;
    self->field_0008 = 2;
}

static void NetGatheringDdl_HandlePhaseFive(S_006a7520* self)
{
    void* resource = reinterpret_cast<void*>(self->field_0090);
    if (CSF_AT(uint32_t, resource, 0xC) == 1)
        return;

    if (resource)
    {
        NetGatheringDdl_ReleaseResource(resource);
        self->field_0090 = 0;
    }

    self->field_000c = 0;
    self->field_0334 = 0;
    self->field_0008 = 2;
}

static void __fastcall NetGatheringDdl_DispatchByPhase(
    S_006a7520* self, void* /*edx*/, int a2)
{
    // Code-address annotation: function value, not a vtable.
    [[maybe_unused]] static void* const referencedVtable = reinterpret_cast<void**>(CSF_ADDR_FN(0x006A7520));

    if (self->field_0008 != 3 || self->field_000c != 5)
        return;

    switch (self->field_0018)
    {
    case 0u:
        self->field_02b0 = 1229730353;
        self->field_0008 = 2;
        self->field_000c = 0;
        self->field_0334 = 0;
        return;
    case 1u:
        NetGatheringDdl_HandlePhaseOne(self);
        return;
    case 2u:
        sub_6A7520(self, nullptr, a2);
        return;
    case 3u:
        sub_6AAD80(self, nullptr, a2);
        return;
    case 5u:
        NetGatheringDdl_HandlePhaseFive(self);
        return;
    default:
        return;
    }
}

HOOK(0x6AAFE0, NetGatheringDdl_DispatchByPhase, "auto");
