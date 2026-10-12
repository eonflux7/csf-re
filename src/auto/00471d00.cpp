#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_FindFocusEntry, 0x0044FBF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_FindSceneEntry, 0x00456650, (S_00452980* self, void* , char* String1));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int*, __fastcall, C_0086c288_GetPointPosition, 0x0075D020, (C_0086c288* self, void* , int* arg0, int a2, int a4));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static std::uint32_t* __fastcall MissionScript_ProcessEntry(void* self, void* /*edx*/)
{
    alignas(8) std::uint8_t temporaryBytes[0x1C];
    CSF_AT(std::uint32_t, temporaryBytes, 0x18) = 0;

    auto* result = CSF_AT(std::uint32_t*, self, 0x08);
    if (result != reinterpret_cast<std::uint32_t*>(static_cast<std::intptr_t>(-1)))
        return result;

    void* entry = CSF_AT(void*, self, 0x04);
    const unsigned entryType = CSF_AT(std::uint8_t, entry, 0);

    switch (entryType)
    {
    case 4:
    {
        const int focusId = CSF_AT(int, entry, 0x10);
        int focusEntry = 0;
        if (focusId != -1)
            focusEntry = SceneLoader_FindFocusEntry(g_pCurrentScene, nullptr, focusId);

        if (focusEntry != 0)
        {
            result = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uint8_t*>(focusEntry) + 0x1C);
        }
        else
        {
            CSF_AT(void**, temporaryBytes, 0x0C) = CSF_VTBL(0x0082D448);
            CSF_AT(int, temporaryBytes, 0x10) = -1;
            CSF_AT(int, temporaryBytes, 0x14) = -1;
            result = reinterpret_cast<std::uint32_t*>(temporaryBytes + 0x0C);
        }

        void* destination = CSF_AT(void*, self, 0x04);
        CSF_AT(std::uint32_t, destination, 0x08) = result[1];
        CSF_AT(std::uint32_t, destination, 0x0C) = result[2];
        return result;
    }

    case 5:
    {
        entry = CSF_AT(void*, self, 0x04);
        void* services = reinterpret_cast<void*>(GetValue8B0554());
        const int pointIndex = CSF_AT(int, entry, 0x14);
        const int pointId = CSF_AT(int, entry, 0x10);
        auto* pointProvider = CSF_AT(C_0086c288*, services, 0x1FD8);
        void* destination = CSF_AT(void*, self, 0x04);

        result = reinterpret_cast<std::uint32_t*>(
            C_0086c288_GetPointPosition(
                pointProvider, nullptr,
                reinterpret_cast<int*>(temporaryBytes), pointId, pointIndex));

        CSF_AT(std::uint32_t, destination, 0x04) = result[0];
        CSF_AT(std::uint32_t, destination, 0x08) = result[1];
        CSF_AT(std::uint32_t, destination, 0x0C) = result[2];
        return result;
    }

    case 11:
    {
        char* name;
        if (CSF_AT(std::uint32_t, entry, 0x1C) < 0x10u)
            name = reinterpret_cast<char*>(entry) + 0x08;
        else
            name = CSF_AT(char*, entry, 0x08);

        result = reinterpret_cast<std::uint32_t*>(
            SceneLoader_FindSceneEntry(
                reinterpret_cast<S_00452980*>(g_pCurrentScene), nullptr, name));
        CSF_AT(std::uint32_t*, entry, 0x20) = result;
        return result;
    }

    default:
        return reinterpret_cast<std::uint32_t*>(
            static_cast<std::intptr_t>(static_cast<int>(entryType) - 11));
    }
}

HOOK(0x00471D00, MissionScript_ProcessEntry, "auto");
