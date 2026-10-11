#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, SkinBindSubobject, 0x006C8390, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, ComputeSkinFloatMetric, 0x006D2A80, (void* self, void* edx));

static void __fastcall SkinGeom_InitFromSource(void* self, void* /*edx*/, int sourceHandle, char flagByte)
{
    CSF_AT(int, self, 0xF0) = sourceHandle;

    int globalInt = GetGlobalInt();
    int clumpManager = ResolveClumpManager(globalInt, nullptr);
    ResolveEntryLookupValue(reinterpret_cast<void*>(clumpManager), nullptr);
    SceneLoader_NoOp();

    SkinBindSubobject(CSF_AT(void*, self, 0x4), nullptr, 0);

    // First virtual call: sourceObj->vtbl[4]() -> lookup object
    void* sourceObj = CSF_AT(void*, self, 0xF0);
    void* lookupObj = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, sourceObj, 0)[4])(sourceObj, nullptr);

    // Second virtual call: lookupObj->vtbl[4]() -> pointer to three dwords
    void* dataObj = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, lookupObj, 0)[4])(lookupObj, nullptr);
    uint32_t* values = static_cast<uint32_t*>(dataObj);

    CSF_AT(uint32_t, self, 0x124) = values[0];
    CSF_AT(uint32_t, self, 0x128) = values[1];
    CSF_AT(uint32_t, self, 0x12C) = values[2];

    CSF_AT(float, self, 0x130) = static_cast<float>(
        ComputeSkinFloatMetric(CSF_AT(void*, self, 0xF0), nullptr));
    CSF_AT(char, self, 0x134) = flagByte;
}

HOOK(0x6C5A40, SkinGeom_InitFromSource, "auto");
