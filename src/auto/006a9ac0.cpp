#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetGatheringDdl_RequestResource, 0x00798FC0,
         (void* self, void* edx, void* a2, int a3, int a4, int a5));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static void __fastcall NetGatheringDdl_SelectGathering(
    void* self, void* /*edx*/, void* selection)
{
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);

    if (!selection)
        return;

    void* sentinel = CSF_AT(void*, self, 0x44);
    const std::uint32_t requestedIndex =
        CSF_AT(std::uint32_t, selection, 0x2B4);
    void* node = CSF_AT(void*, sentinel, 0);
    void* selectedEntry = nullptr;

    if (node != sentinel)
    {
        std::uint32_t index = 0;
        do
        {
            if (index == requestedIndex)
                break;

            node = CSF_AT(void*, node, 0);
            ++index;
        } while (node != CSF_AT(void*, self, 0x44));

        if (node != sentinel)
            selectedEntry = static_cast<std::uint8_t*>(node) + 8;
    }

    CSF_AT(void*, self, 0x32C) = selectedEntry;
    if (!selectedEntry)
    {
        CSF_AT(std::int32_t, self, 0x2B0) = 1229730105;
        return;
    }

    void* entryResource = CSF_AT(void*, selectedEntry, 4);
    CSF_AT(std::int32_t, self, 0x8C) =
        CSF_AT(std::int32_t, entryResource, 4);
    auto* completionFlag = static_cast<std::uint8_t*>(self) + 0x2B4;
    *completionFlag = 0;

    void* allocation = NetZ_OperatorNew(112);
    void* resource = allocation ? ConstructResource(allocation, nullptr) : nullptr;
    CSF_AT(void*, self, 0x90) = resource;

    // The original reuses its four-byte argument slot as the temporary string.
    InitializeTemporaryString(
        &selection, nullptr, reinterpret_cast<std::int32_t>(&g_byte_82E65C));

    const int resourceId = CSF_AT(std::int32_t, self, 0x8C);
    void* requestResource = CSF_AT(void*, self, 0x90);
    void* requestOwner = CSF_AT(void*, self, 0x2C);
    const char accepted = NetGatheringDdl_RequestResource(
        requestOwner, nullptr, requestResource, resourceId,
        reinterpret_cast<int>(completionFlag),
        reinterpret_cast<int>(&selection));

    DestroyTemporaryString(&selection, nullptr);

    if (accepted)
    {
        CSF_AT(std::int32_t, self, 0x08) = 3;
        CSF_AT(std::int32_t, self, 0x0C) = 5;
        CSF_AT(std::int32_t, self, 0x18) = 1;
    }
    else
    {
        void* failedResource = CSF_AT(void*, self, 0x90);
        CSF_AT(std::int32_t, self, 0x2B0) = 1229730867;
        if (failedResource)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, failedResource, 0)[0])(
                    failedResource, nullptr, 1);
            CSF_AT(void*, self, 0x90) = nullptr;
        }
    }
}

HOOK(0x006A9AC0, NetGatheringDdl_SelectGathering, "auto");
