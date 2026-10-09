#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, FindSceneSlotIndexById, 0x00468E20, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, BindRegionToSceneSlot, 0x0046A990, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, GetEntityComponent, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SetRegionLinkMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, AttachRegionToEntity, 0x004FBBE0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetSceneRegionCount, 0x004FC8B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetSceneEntityAt, 0x004FC140, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, AddSceneRegion, 0x004FC970, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

using GetIntFromObjectFn = int(__fastcall*)(void* self, void* edx);
using SetIntPairOnObjectFn = void(__fastcall*)(void* self, void* edx, int first, int second);

static void* AsObjectPointer(uint32_t bits) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(bits));
}

static C_0085ec18* AsRegionPointer(uint32_t bits) {
    return reinterpret_cast<C_0085ec18*>(static_cast<uintptr_t>(bits));
}

static void* GetRegionStore() {
    return AsObjectPointer(static_cast<uint32_t>(GetGlobalValue_4FBDE0()));
}

static C_0085ec18* __stdcall FindOrCreateRegionForSceneId(int /*a1*/, int /*a2*/, uint32_t* records) {
    // Scene id: records[0] -> node, node+4 -> block, block+4 holds the id as a float.
    void* idNode = AsObjectPointer(records[0]);
    void* idBlock = CSF_AT(void*, idNode, 4);
    int sceneId = static_cast<int>(CSF_AT(float, idBlock, 4));

    // Scene manager sub-object: *(g_pCurrentScene + 0xBC) + 0x94.
    void* currentScene = g_pCurrentScene;
    void* sceneManager = AsObjectPointer(CSF_AT(uint32_t, currentScene, 0xBC) + 0x94);

    int existingIndex = FindSceneSlotIndexById(sceneManager, currentScene, sceneId);
    if (existingIndex != -1) {
        return AsRegionPointer(static_cast<uint32_t>(existingIndex));
    }

    // Width and height of the global object: LookupObjectById(GetGlobalInt()), virtual slots 8 and 9.
    void* globalObjectForWidth = LookupObjectById(AsObjectPointer(static_cast<uint32_t>(GetGlobalInt())), nullptr);
    int globalWidth = ((GetIntFromObjectFn)CSF_AT(void**, globalObjectForWidth, 0)[8])(globalObjectForWidth, nullptr);
    void* globalObjectForHeight = LookupObjectById(AsObjectPointer(static_cast<uint32_t>(GetGlobalInt())), nullptr);
    int globalHeight = ((GetIntFromObjectFn)CSF_AT(void**, globalObjectForHeight, 0)[9])(globalObjectForHeight, nullptr);

    // Scale vectors: records[1] and records[2] -> record, record+4 -> vector {x @ +4, y @ +8}.
    void* minScaleVector = CSF_AT(void*, AsObjectPointer(records[1]), 4);
    float minScaleX = CSF_AT(float, minScaleVector, 4);
    float minScaleY = CSF_AT(float, minScaleVector, 8);
    void* maxScaleVector = CSF_AT(void*, AsObjectPointer(records[2]), 4);
    float maxScaleX = CSF_AT(float, maxScaleVector, 4);
    float maxScaleY = CSF_AT(float, maxScaleVector, 8);

    float widthF = static_cast<float>(globalWidth);
    float heightF = static_cast<float>(globalHeight);
    int regionLeft = static_cast<int>(widthF * minScaleX);
    int regionTop = static_cast<int>(heightF * minScaleY);
    int regionRight = static_cast<int>(widthF * maxScaleX);
    int regionBottom = static_cast<int>(heightF * maxScaleY);

    // Create the region in the global region store.
    int newRegionIndex = AddSceneRegion(GetRegionStore(), nullptr, 1, regionLeft, regionTop, regionRight, regionBottom);
    if (newRegionIndex == -1) {
        return AsRegionPointer(static_cast<uint32_t>(newRegionIndex));
    }

    int lastRegionIndex = GetSceneRegionCount(GetRegionStore(), nullptr);
    void* regionRecord = AsObjectPointer(static_cast<uint32_t>(GetSceneRegionAt(GetRegionStore(), nullptr, lastRegionIndex)));
    SetRegionLinkMode(regionRecord, nullptr, 4);

    void* regionEntity = AsObjectPointer(static_cast<uint32_t>(
        GetSceneEntityAt(GetRegionStore(), nullptr, static_cast<unsigned int>(newRegionIndex))));
    if (regionEntity == nullptr) {
        return nullptr;
    }
    AttachRegionToEntity(regionEntity, nullptr, regionRecord);

    void* firstComponent = AsObjectPointer(static_cast<uint32_t>(GetEntityComponent(regionEntity, nullptr)));
    ((SetIntPairOnObjectFn)CSF_AT(void**, firstComponent, 0)[19])(firstComponent, nullptr, 2, 0);
    void* secondComponent = AsObjectPointer(static_cast<uint32_t>(GetEntityComponent(regionEntity, nullptr)));
    ((SetIntPairOnObjectFn)CSF_AT(void**, secondComponent, 0)[19])(secondComponent, nullptr, 16, 1);

    return AsRegionPointer(BindRegionToSceneSlot(sceneManager, nullptr, sceneId, newRegionIndex));
}

HOOK(0x0045DCA0, FindOrCreateRegionForSceneId, "auto");
