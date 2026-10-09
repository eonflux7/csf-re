#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_476C60, 0x00476C60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, sub_485940, 0x00485940, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_485980, 0x00485980, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, sub_755810, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, sub_760F90, 0x00760F90, (int a1));

// Float -> int conversion as done by the original's __ftol2 call: truncate toward zero to a
// 64-bit integer, then keep only the low 32 bits. Values outside the int64 range (and NaN)
// yield the x87 "integer indefinite" value 0x8000000000000000, whose low 32 bits are 0.
static int TruncateFloatToLowInt32(float value)
{
    constexpr float kInt64Limit = 9223372036854775808.0f; // 2^63
    if (!(value > -kInt64Limit && value < kInt64Limit))
        return 0;
    const int64_t wide = static_cast<int64_t>(value);
    return static_cast<int>(static_cast<uint32_t>(static_cast<uint64_t>(wide)));
}

// 0x00461550: resolves the scene entity referenced by argRefs[0]/argRefs[1], converts the
// two float coordinates from argRefs[2]/argRefs[3] and forwards them to the entity's FSM.
static int __stdcall DispatchPositionToSceneEntity(int /*unusedA1*/, int /*unusedA2*/, uint32_t* argRefs)
{
    // The record at argRefs[0]+4; its signed int at +12 selects whether the entity is handled.
    const int recordAddress = CSF_AT(int, reinterpret_cast<void*>(argRefs[0]), 4);
    const int recordIndex = CSF_AT(int, reinterpret_cast<void*>(recordAddress), 12);
    int result = recordAddress + 4;
    if (recordIndex < 0)
        return result;

    // Resolve the scene entity from the handle address.
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
    const int entity = result;
    if (entity == 0)
        return result;

    // The entity must pass the flag check before anything else happens.
    result = sub_755810(CSF_AT(uint32_t, reinterpret_cast<void*>(entity), 80));
    if (static_cast<uint8_t>(result) == 0)
        return result;

    // Query the services object for the target index.
    const int servicesBase = GetValue8B0554();
    void* lookupService = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(servicesBase), 8128));
    const int sourceHandle = CSF_AT(int,
        reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(argRefs[1]), 4)), 4);
    result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, lookupService, 0)[3])(
        lookupService, nullptr, sourceHandle);
    const int targetIndex = result;
    if (targetIndex == -1)
        return result;

    // Coordinates: float at (*(argRefs[2]+4))+4 and (*(argRefs[3]+4))+4, truncated to int
    // through the 64-bit truncation helper (low 32 bits kept).
    const float xValue = CSF_AT(float,
        reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(argRefs[2]), 4)), 4);
    const int firstCoord = TruncateFloatToLowInt32(xValue);
    const float yValue = CSF_AT(float,
        reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(argRefs[3]), 4)), 4);
    const int secondCoord = TruncateFloatToLowInt32(yValue);

    // Choose the direct CCharActionFsm path only when the entity has no link at +80 and
    // the entry object at the target index passes sub_760F90.
    bool useDirectFsmCalls = false;
    if (CSF_AT(int, reinterpret_cast<void*>(entity), 80) == 0) {
        const int servicesBaseAgain = GetValue8B0554();
        void* lookupServiceAgain = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(servicesBaseAgain), 8128));
        const int entryObject = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, lookupServiceAgain, 0)[2])(
            lookupServiceAgain, nullptr, targetIndex);
        useDirectFsmCalls = static_cast<uint8_t>(
            sub_760F90(CSF_AT(int, reinterpret_cast<void*>(entryObject), 116))) != 0;
    }

    void* fsm = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(entity), 68));

    if (useDirectFsmCalls) {
        result = sub_485980(fsm, nullptr, targetIndex);
        if (result != -1) {
            void* node = reinterpret_cast<void*>(sub_485940(fsm, nullptr, result));
            return sub_476C60(node, nullptr, firstCoord, secondCoord);
        }
        return result;
    }

    result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, fsm, 0)[48])(
        fsm, nullptr, targetIndex);
    if (result == -1)
        return result;
    void* node = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, fsm, 0)[50])(fsm, nullptr, result));
    return sub_476C60(node, nullptr, firstCoord, secondCoord);
}

HOOK(0x00461550, DispatchPositionToSceneEntity, "auto");
