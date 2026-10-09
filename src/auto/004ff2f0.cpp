#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RpWorldForAllMaterials, 0x0058CF30, (int a1, int (__cdecl* a2)(uint32_t, int), int a3));
CSF_FUNC(int, __cdecl, RpWorldForAllClumps, 0x0058CEF0, (int a1, int (__cdecl* a2)(uint32_t*, int), int a3));

static int __cdecl SceneLoader_MaterialFlagCallback(uint32_t material, int flagPtr);
static int __cdecl SceneLoader_ClumpFlagCallback(uint32_t* clump, int flagPtr);

static int __cdecl SceneLoader_MaterialFlagCallback(uint32_t material, int flagPtr)
{
    (void)material;
    (void)flagPtr;
    return 0;
}

static int __cdecl SceneLoader_ClumpFlagCallback(uint32_t* clump, int flagPtr)
{
    (void)clump;
    (void)flagPtr;
    return 0;
}

static int __stdcall SceneLoader_ApplyFlagToWorldMaterialsAndClumps(int worldHandle, char flagValue)
{
    char flag[4];
    flag[0] = flagValue;

    // First lookup: global id -> object, then virtual slot 6 (+0x18) -> read +0x20
    void* firstObject = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* firstResult = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, firstObject, 0)[6])(firstObject, nullptr);
    int firstValue = CSF_AT(int, firstResult, 0x20);
    (void)firstValue;

    // Second lookup: global id -> object, then virtual slot 6 (+0x18) -> read +0x1C
    void* secondObject = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* secondResult = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, secondObject, 0)[6])(secondObject, nullptr);
    int secondValue = CSF_AT(int, secondResult, 0x1C);
    (void)secondValue;

    RpWorldForAllMaterials(
        worldHandle,
        reinterpret_cast<int (__cdecl*)(uint32_t, int)>(CSF_ADDR_FN(0x004FEE70)),
        reinterpret_cast<int>(flag));
    return RpWorldForAllClumps(
        worldHandle,
        reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x004FF000)),
        reinterpret_cast<int>(flag));
}

HOOK(0x004FF2F0, SceneLoader_ApplyFlagToWorldMaterialsAndClumps, "auto");
