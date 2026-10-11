#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_008656d0_IsMaskSubset, 0x006C5780, (C_008656d0* self, void* /*edx*/, int mask));
CSF_FUNC(char, __fastcall, sub_6B7930, 0x006B7930, (void* self, void* edx, uint8_t a2));
CSF_FUNC(char, __fastcall, sub_6BD7F0, 0x006BD7F0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_6CC8E0, 0x006CC8E0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_6DB280, 0x006DB280, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_6F61B0, 0x006F61B0, (void* self, void* edx));

static int __fastcall GfxScene_SyncWorldClumps(S_006cbc80* self, void* /*edx*/)
{
    int globalValueHandle = GetGlobalValue_6C5760();
    auto* globalValue = reinterpret_cast<C_008656d0*>(static_cast<uintptr_t>(globalValueHandle));
    int result = C_008656d0_IsMaskSubset(globalValue, nullptr, 256);
    if (static_cast<uint8_t>(result) != 0)
    {
        if (self->field_006b != 0)
            sub_6B7930(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c)), nullptr,
                       self->field_006c);
        else
            sub_6BD7F0(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr);

        sub_6CC8E0(self, nullptr);

        int globalValueHandle2 = GetGlobalValue_6C5760();
        auto* globalValue2 = reinterpret_cast<C_008656d0*>(static_cast<uintptr_t>(globalValueHandle2));
        result = C_008656d0_IsMaskSubset(globalValue2, nullptr, 128);
        if (static_cast<uint8_t>(result) != 0)
        {
            int globalInt = GetGlobalInt();
            int clumpManager = ResolveClumpManager(globalInt, nullptr);
            int entryLookup = ResolveEntryLookupValue(
                reinterpret_cast<void*>(static_cast<uintptr_t>(clumpManager)), nullptr);
            result = sub_6DB280(reinterpret_cast<void*>(static_cast<uintptr_t>(entryLookup)), nullptr);
        }

        if (self->field_00c4 != 0)
            return sub_6F61B0(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_00c4)), nullptr);
    }
    return result;
}

HOOK(0x006CD190, GfxScene_SyncWorldClumps, "auto");
