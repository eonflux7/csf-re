#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_006a7640_FinishOperation, 0x006A7860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006a7640_ProcessGatheringResult, 0x006A7B50, (void* self, void* edx, int a2, int* a3));
CSF_FUNC(char, __fastcall, NetGatheringDdl_CheckGathering, 0x00791790, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, NetGatheringDdl_StartResourceRequest, 0x0079E520, (void* self, void* edx, void* a2, int a5, void** a4));

static void __fastcall S_006a7640_HandleResourceCompletion(
    S_006a7640* self, void* /*edx*/, int /*unused*/)
{
    auto* resource = reinterpret_cast<C_0085e760*>(self->field_0090);

    switch (CSF_AT(std::uint32_t, resource, 0x0C))
    {
    case 1:
        return;

    case 2:
        if (self->field_0038)
        {
            if (resource)
            {
                resource->vSlot0(1);
                self->field_0090 = 0;
            }

            if (!NetGatheringDdl_CheckGathering(
                    reinterpret_cast<void*>(self->field_0034), nullptr,
                    reinterpret_cast<uint32_t*>(self->field_0038)))
            {
                self->field_02b0 = 1229730102;
                S_006a7640_FinishOperation(self, nullptr);
                return;
            }

            void* storage = NetZ_OperatorNew(112);
            void* nextResource = storage
                ? ConstructResource(storage, nullptr)
                : nullptr;
            self->field_0090 = reinterpret_cast<std::uint32_t>(nextResource);

            const char* name;
            if (CSF_AT(std::uint32_t, self, 0x300) < 0x10u)
                name = reinterpret_cast<const char*>(self) + 0x2EC;
            else
                name = CSF_AT(const char*, self, 0x2EC);

            alignas(8) std::uint8_t temporaryStringBytes[0x4];
            InitializeTemporaryString(
                temporaryStringBytes, nullptr,
                reinterpret_cast<std::int32_t>(name));

            const bool failed = NetGatheringDdl_StartResourceRequest(
                reinterpret_cast<void*>(self->field_0034), nullptr,
                reinterpret_cast<void*>(self->field_0090),
                reinterpret_cast<int>(
                    reinterpret_cast<std::uint8_t*>(self) + 0xAC),
                reinterpret_cast<void**>(temporaryStringBytes)) == 0;

            DestroyTemporaryString(temporaryStringBytes, nullptr);

            if (!failed)
            {
                self->field_0024 = 1;
                return;
            }

            resource = reinterpret_cast<C_0085e760*>(self->field_0090);
            self->field_02b0 = 1229730608;
            if (resource)
            {
                resource->vSlot0(1);
                self->field_0090 = 0;
            }
        }
        else
        {
            if (resource)
            {
                resource->vSlot0(1);
                self->field_0090 = 0;
            }
            S_006a7640_ProcessGatheringResult(
                self, nullptr, 0,
                reinterpret_cast<int*>(
                    reinterpret_cast<std::uint8_t*>(self) + 0x94));
        }
        break;

    case 3:
        if (resource)
        {
            resource->vSlot0(1);
            self->field_0090 = 0;
        }

        S_006a7640_ProcessGatheringResult(
            self, nullptr, 0,
            reinterpret_cast<int*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x94));

        resource = reinterpret_cast<C_0085e760*>(self->field_0090);
        if (resource)
        {
            resource->vSlot0(1);
            self->field_0090 = 0;
        }
        break;

    default:
        if (resource)
        {
            resource->vSlot0(1);
            self->field_0090 = 0;
        }
        self->field_02b0 = 1229730103;
        break;
    }

    S_006a7640_FinishOperation(self, nullptr);
}

HOOK(0x006A90B0, S_006a7640_HandleResourceCompletion, "auto");
