#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, GfxScene_UpdateClump, 0x006B4D10, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, SceneLoader_ProcessContextObject, 0x006BE9C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_008656d0_IsMaskSubset, 0x006C5780, (C_008656d0* self, void* , int mask));
CSF_FUNC(char, __fastcall, GfxScene_UpdateAuxiliaryObject, 0x006D5A00, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, GfxScene_AdvanceElapsedTime, 0x006D9D00, (void* self, void* edx, float a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_UpdatePrimaryObject, 0x006EF490, (void* self, void* edx, float a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_UpdateListedObject, 0x006F7490, (void* self, void* edx, float a2));

static char __fastcall GfxScene_Update(void* self, void* /*edx*/, float deltaTime)
{
    void* primaryObject = CSF_AT(void*, self, 0x64);
    if (primaryObject)
        GfxScene_UpdatePrimaryObject(primaryObject, nullptr, deltaTime);

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x5C); ++i)
    {
        void* object = CSF_AT(void**, self, 0x54)[i];
        GfxScene_UpdateListedObject(object, nullptr, deltaTime);
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x1C); ++i)
    {
        C_00860288* clump = CSF_AT(C_00860288**, self, 0x14)[i];
        if (clump && static_cast<uint8_t>(clump->vSlot38()))
            GfxScene_UpdateClump(clump, nullptr, deltaTime);
    }

    auto* groupCount = static_cast<uint8_t*>(self) + 0x2C;
    int remainingGroups = 3;
    do
    {
        for (uint32_t i = 0; i < CSF_AT(uint32_t, groupCount, 0); ++i)
        {
            void* clump = CSF_AT(void**, groupCount, -8)[i];
            GfxScene_UpdateClump(clump, nullptr, deltaTime);
        }
        groupCount += 0x10;
        --remainingGroups;
    } while (remainingGroups);

    void* auxiliaryObject = CSF_AT(void*, self, 0xBC);
    if (auxiliaryObject)
    {
        int deltaTimeBits;
        std::memcpy(&deltaTimeBits, &deltaTime, sizeof(deltaTimeBits));
        GfxScene_UpdateAuxiliaryObject(auxiliaryObject, nullptr, deltaTimeBits);
    }

    auto* globalState =
        reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
    char result = static_cast<char>(
        C_008656d0_IsMaskSubset(globalState, nullptr, 128));

    if (result)
    {
        int managerId = GetGlobalInt();
        int manager = ResolveClumpManager(managerId, nullptr);
        int entry = ResolveEntryLookupValue(
            reinterpret_cast<void*>(manager), nullptr);
        GfxScene_AdvanceElapsedTime(
            reinterpret_cast<void*>(entry), nullptr, deltaTime);

        // The timer-only callee has no declared return value; the legacy
        // byte result on this path is the low byte of the lookup result.
        result = static_cast<char>(entry);
    }

    auto* context = CSF_AT(C_008605d8*, self, 0x98);
    if (context)
        return SceneLoader_ProcessContextObject(context, nullptr);

    return result;
}

HOOK(0x006CB500, GfxScene_Update, "auto");
