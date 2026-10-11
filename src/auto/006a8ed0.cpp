#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_006a7640_FinalizeGatheringRequest, 0x006A78D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006a7640_ReportGatheringResult, 0x006A7B50, (void* self, void* edx, int a2, int* a3));
CSF_FUNC(char, __fastcall, NetGatheringDdl_ValidateGathering, 0x00791790, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, NetGatheringDdl_StartGatheringRequest, 0x0079E420, (void* self, void* edx, void* a2, int a8, void** a4, void** a5, int a6, void** a7));

static void __fastcall S_006a7640_ProcessGatheringRequest(
    S_006a7640* self, void* /*edx*/, int /*unused*/)
{
    auto* resource = reinterpret_cast<C_0085e760*>(self->field_0090);

    // The original reads the resource state before checking the pointer.
    switch (CSF_AT(std::uint32_t, resource, 0x0C))
    {
    case 1:
        return;

    case 2:
    {
        if (resource)
        {
            resource->vSlot0(1);
            self->field_0090 = 0;
        }

        if (self->field_0038)
        {
            if (!NetGatheringDdl_ValidateGathering(
                    reinterpret_cast<void*>(self->field_0034), nullptr,
                    reinterpret_cast<uint32_t*>(self->field_0038)))
            {
                self->field_02b0 = 1229730102;
                S_006a7640_FinalizeGatheringRequest(self, nullptr);
                return;
            }

            void* allocation = NetZ_OperatorNew(112);
            void* newResource = allocation
                ? ConstructResource(allocation, nullptr)
                : nullptr;
            self->field_0090 = reinterpret_cast<std::uint32_t>(newResource);

            alignas(8) std::uint8_t temporaryStrings[0xC];
            auto* bytes = reinterpret_cast<std::uint8_t*>(self);

            const char* source = CSF_AT(std::uint32_t, self, 0x31C) < 0x10u
                ? reinterpret_cast<const char*>(bytes + 0x308)
                : CSF_AT(const char*, self, 0x308);
            InitializeTemporaryString(
                temporaryStrings, nullptr, reinterpret_cast<std::int32_t>(source));

            source = CSF_AT(std::uint32_t, self, 0x300) < 0x10u
                ? reinterpret_cast<const char*>(bytes + 0x2EC)
                : CSF_AT(const char*, self, 0x2EC);
            InitializeTemporaryString(
                temporaryStrings + 4, nullptr, reinterpret_cast<std::int32_t>(source));

            source = CSF_AT(std::uint32_t, self, 0x2E4) < 0x10u
                ? reinterpret_cast<const char*>(bytes + 0x2D0)
                : CSF_AT(const char*, self, 0x2D0);
            InitializeTemporaryString(
                temporaryStrings + 8, nullptr, reinterpret_cast<std::int32_t>(source));

            const bool failed = NetGatheringDdl_StartGatheringRequest(
                reinterpret_cast<void*>(self->field_0034), nullptr,
                reinterpret_cast<void*>(self->field_0090),
                reinterpret_cast<int>(bytes + 0xA0),
                reinterpret_cast<void**>(temporaryStrings + 8),
                reinterpret_cast<void**>(temporaryStrings + 4),
                0, reinterpret_cast<void**>(temporaryStrings)) == 0;

            DestroyTemporaryString(temporaryStrings + 8, nullptr);
            DestroyTemporaryString(temporaryStrings + 4, nullptr);
            DestroyTemporaryString(temporaryStrings, nullptr);

            if (!failed)
            {
                self->field_0020 = 1;
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
            S_006a7640_ReportGatheringResult(
                self, nullptr, 1,
                reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(self) + 0x94));
        }

        S_006a7640_FinalizeGatheringRequest(self, nullptr);
        return;
    }

    case 3:
        if (resource)
        {
            resource->vSlot0(1);
            self->field_0090 = 0;
        }
        S_006a7640_ReportGatheringResult(
            self, nullptr, 3,
            reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(self) + 0x94));
        S_006a7640_FinalizeGatheringRequest(self, nullptr);
        return;

    default:
        if (resource)
        {
            resource->vSlot0(1);
            self->field_0090 = 0;
        }
        self->field_02b0 = 1229730103;
        S_006a7640_FinalizeGatheringRequest(self, nullptr);
        return;
    }
}

HOOK(0x006A8ED0, S_006a7640_ProcessGatheringRequest, "auto");
