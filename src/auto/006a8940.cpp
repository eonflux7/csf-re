#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetGatheringDdl_RequestResource, 0x00798FF0, (void* self, void* edx, void* a2, int a3, int a4, int a5));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static void __fastcall S_006a8940_InitializeGathering(
    S_006a8940* self, void* /*edx*/)
{
    alignas(8) std::uint8_t temporaryBytes[8];
    const std::uint32_t state = self->field_0014;
    temporaryBytes[7] = 0;

    if (state == 1)
    {
        self->field_0008 = 2;
        self->field_000c = 0;
        self->field_0334 = 0;
        return;
    }

    if (state != 0)
        return;

    self->field_0014 = 1;
    void* allocation = NetZ_OperatorNew(112);
    void* resource = allocation ? ConstructResource(allocation, nullptr) : nullptr;
    self->field_0090 = reinterpret_cast<std::uint32_t>(resource);

    InitializeTemporaryString(
        temporaryBytes, nullptr,
        reinterpret_cast<std::int32_t>(&g_byte_82E65C));

    const int resourceId = CSF_AT(
        int, reinterpret_cast<void*>(self->field_0080), 0x4);
    void* requester = reinterpret_cast<void*>(self->field_002c);
    void* requestedResource = reinterpret_cast<void*>(self->field_0090);

    const bool failed = NetGatheringDdl_RequestResource(
        requester, nullptr, requestedResource, resourceId,
        reinterpret_cast<int>(temporaryBytes + 7),
        reinterpret_cast<int>(temporaryBytes)) == 0;

    DestroyTemporaryString(temporaryBytes, nullptr);

    if (failed)
    {
        auto* failedResource =
            reinterpret_cast<C_0085e760*>(self->field_0090);
        self->field_02b0 = 1229730100;

        if (failedResource)
        {
            failedResource->vSlot0(1);
            self->field_0090 = 0;
        }

        S_006a8940_InitializeGathering(self, nullptr);
    }
}

HOOK(0x006A8940, S_006a8940_InitializeGathering, "auto");
