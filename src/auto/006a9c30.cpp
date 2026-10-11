#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(int, __fastcall, NetGatheringDdl_PrepareRequestData, 0x00652810, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetGatheringDdl_StartResourceRequest, 0x00799070, (void* self, void* edx, void* a2, int a3, int a4));

static int __fastcall NetGatheringDdl_UpdateResourceRequest(
    void* self, void* /*edx*/, int /*unused*/)
{
    auto* resource = CSF_AT(C_0085e760*, self, 0x38);
    const uint32_t resourceState = CSF_AT(uint32_t, resource, 0x0C);

    switch (resourceState)
    {
    case 1:
        return 0;

    case 2:
    {
        if (resource)
        {
            resource->vSlot0(1);
            CSF_AT(C_0085e760*, self, 0x38) = nullptr;
        }

        auto* destination = CSF_AT(uint32_t*, self, 0x34);
        *destination = CSF_AT(uint32_t, self, 0x20);

        void* requestData = static_cast<uint8_t*>(self) + 8;
        NetGatheringDdl_PrepareRequestData(requestData, nullptr);

        void* storage = NetZ_OperatorNew(112);
        auto* replacement = storage
            ? static_cast<C_0085e760*>(ConstructResource(storage, nullptr))
            : nullptr;

        void* context = CSF_AT(void*, self, 0x40);
        CSF_AT(C_0085e760*, self, 0x38) = replacement;
        void* contextEntry = CSF_AT(void*, context, 0x04);
        int requestValue = CSF_AT(int, contextEntry, 0x04);

        const int result = NetGatheringDdl_StartResourceRequest(
            CSF_AT(void*, self, 0x3C), nullptr, replacement,
            requestValue, reinterpret_cast<int>(requestData)) != 0 ? 2 : 0;
        CSF_AT(int, self, 0x04) = result;
        return result;
    }

    case 3:
    default:
    {
        int result = static_cast<int>(resourceState - 3);
        if (resource)
        {
            result = reinterpret_cast<int>(resource->vSlot0(1));
            CSF_AT(C_0085e760*, self, 0x38) = nullptr;
        }
        CSF_AT(int, self, 0x04) = 0;
        return result;
    }
    }
}

HOOK(0x006A9C30, NetGatheringDdl_UpdateResourceRequest, "auto");
