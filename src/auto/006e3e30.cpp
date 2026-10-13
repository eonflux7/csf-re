#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResolveResource, 0x006D59B0, (void* self, void* edx, char* String2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSector, 0x006D4720, (void* self, void* edx, int a2));

// Copies the 0x40-byte resource block referenced by a scene sector entry into
// self[2], then forwards two stored values to the object's virtual slot 5.
static bool __fastcall C_00861a78_ConfigureFromEntry(void** self, void* /*edx*/, uint32_t* a2)
{
    if (a2[0] == 3)
    {
        void* globalInt = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(globalInt, nullptr));
        void* provider = GetCachedObjectProvider(accessor, nullptr);

        char* resourceName = (a2[14] < 0x10u)
            ? reinterpret_cast<char*>(a2 + 9)
            : reinterpret_cast<char*>(static_cast<uintptr_t>(a2[9]));

        uint32_t resourceId = GfxScene_ResolveResource(provider, nullptr, resourceName);

        uint8_t* sectorEntry = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(SceneLoader_GetSector(provider, nullptr, static_cast<int>(resourceId))));

        if (CSF_AT(uint32_t, sectorEntry, 0x28) != 0)
        {
            uint8_t* sectorEntry2 = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(SceneLoader_GetSector(provider, nullptr, static_cast<int>(resourceId))));

            uint8_t* blockPtr = CSF_AT(uint8_t*, sectorEntry2, 0x20);
            uint8_t* dataHeader = CSF_AT(uint8_t*, blockPtr, 0);
            uint32_t dataValue = CSF_AT(uint32_t, dataHeader, 4);
            const uint8_t* src = reinterpret_cast<const uint8_t*>(
                static_cast<uintptr_t>(dataValue) + 0x10);

            std::memcpy(self[2], src, 0x40);
        }
    }

    ((void(__fastcall*)(void*, void*, uint32_t, uint32_t))CSF_AT(void**, self, 0)[5])(
        self, nullptr, a2[23], a2[22]);

    return true;
}

HOOK(0x006E3E30, C_00861a78_ConfigureFromEntry, "auto");
