#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetGatheringDdl_InitializeGatheringResource, 0x00798FF0, (void* self, void* edx, void* a2, int a3, int a4, int a5));
CSF_FUNC(void, __fastcall, NetGatheringDdl_ReleasePendingHandle, 0x0079FC50, (void* self, void* edx, int a2));

CSF_GLOBAL(std::uint8_t, g_EmptyStringByte, 0x0082E65C);

static void __fastcall NetGatheringDdl_CompletePendingTransition(
    void* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x8) != 7)
        return;

    void* resource = CSF_AT(void*, self, 0x90);
    if (resource)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
        CSF_AT(void*, self, 0x90) = nullptr;
    }

    NetGatheringDdl_ReleasePendingHandle(
        CSF_AT(void*, self, 0x330), nullptr,
        CSF_AT(int, self, 0x8C));

    if (!CSF_AT(std::uint8_t, self, 0x2C9))
    {
        void* allocation = NetZ_OperatorNew(112);
        void* replacement = allocation
            ? ConstructResource(allocation, nullptr)
            : nullptr;
        CSF_AT(void*, self, 0x90) = replacement;

        alignas(8) std::uint8_t temporaryStringBytes[0x4];
        InitializeTemporaryString(
            temporaryStringBytes, nullptr,
            reinterpret_cast<std::int32_t>(&g_EmptyStringByte));

        NetGatheringDdl_InitializeGatheringResource(
            CSF_AT(void*, self, 0x2C), nullptr,
            CSF_AT(void*, self, 0x90),
            CSF_AT(int, self, 0x8C),
            reinterpret_cast<int>(
                reinterpret_cast<std::uint8_t*>(self) + 0x2B4),
            reinterpret_cast<int>(temporaryStringBytes));

        DestroyTemporaryString(temporaryStringBytes, nullptr);
    }

    CSF_AT(std::uint8_t, self, 0x2C9) = 0;
    CSF_AT(int, self, 0x8) = 3;
    CSF_AT(int, self, 0xC) = 8;
}

HOOK(0x006A7940, NetGatheringDdl_CompletePendingTransition, "auto");
