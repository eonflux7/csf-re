#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Data referenced by the original.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_DegreesPerRadian, 0x008313E4);

// Callees.
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, HandleTable_FindIndex, 0x007686E0, (void* self, void* edx, int handleId));
CSF_FUNC(int, __fastcall, HandleTable_GetEntryAt, 0x00768740, (void* self, void* edx, int index));
// The original also passes edi (the entry pointer) to this callee; that register is not forwarded (see notes).
CSF_FUNC(char, __fastcall, ObjectProvider_CallWithDefaultOptions, 0x004F53D0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ObjectProvider_TryFindIndexedEntry, 0x004F6E90,
         (void* self, void* edx, int index, uint32_t* outIndex));
// The original also passes edi (the entry pointer) to the two callees below; that register is not forwarded (see notes).
CSF_FUNC(int, __fastcall, ObjectProvider_ConfigureOrderTarget, 0x004F7B40,
         (void* self, void* edx, void* sourceHandle, void* targetHandle, float weight,
          float* targetVector, float* explicitAngle));
CSF_FUNC(int, __fastcall, ObjectProvider_SetOrderState, 0x004F7810, (void* self, void* edx, int orderCode));

// Handle object as built on the stack by the original: {vtable, idA, idB}.
struct RefHandleTriple {
    void** vtbl;
    int32_t field_04;
    int32_t field_08;
};

struct OrderRecord {
    int32_t index;            // ebp-0x3C
    RefHandleTriple source;   // ebp-0x38
    RefHandleTriple target;   // ebp-0x2C
    float weight;             // ebp-0x20
    uint32_t extra[4];        // ebp-0x1C .. ebp-0x10
};
static_assert(sizeof(OrderRecord) == 0x30);

// __stdcall: the original pops 3 stack arguments (ret 12); only the third is used.
static char __stdcall Scene_SyncEntityTransformAndIssueOrder(int /*a4*/, int /*a5*/, void* requestPointer)
{
    // if (!g_pCurrentScene->field_C0->flag_09) return flag;
    void* sceneState = CSF_AT(void*, g_pCurrentScene, 0xC0);
    uint8_t sceneEnabled = CSF_AT(uint8_t, sceneState, 9);
    if (sceneEnabled == 0) {
        return static_cast<char>(sceneEnabled);
    }

    // v7 = *(*(int*)a6 + 4)
    void* requestRecord = CSF_AT(void*, requestPointer, 0);
    void* requestPayload = CSF_AT(void*, requestRecord, 4);

    // v8 = HandleTable_FindIndex(GetValue8B0554()->+0x1F90, *(v7 + 4))
    void* handleTableOwner = reinterpret_cast<void*>(GetValue8B0554());
    int handleId = CSF_AT(int, requestPayload, 4);
    void* handleTable = CSF_AT(void*, handleTableOwner, 0x1F90);
    int handleIndex = HandleTable_FindIndex(handleTable, nullptr, handleId);
    if (handleIndex == -1) {
        // Original returns the low byte of the -1 result.
        return static_cast<char>(handleIndex);
    }

    // entry = HandleTable_GetEntryAt(GetValue8B0554()->+0x1F90, handleIndex)
    void* secondTableOwner = reinterpret_cast<void*>(GetValue8B0554());
    void* secondTable = CSF_AT(void*, secondTableOwner, 0x1F90);
    void* entry = reinterpret_cast<void*>(HandleTable_GetEntryAt(secondTable, nullptr, handleIndex));

    // provider = GetCachedObjectProvider(GetFirstElement(GetGlobalValue_4FBDE0()))
    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* cachedProvider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);

    // positionSink = GetReferencedObject(provider)->vtbl[4]();  positionSink->vtbl[5](entry + 4)
    void* referencedObject = GetReferencedObject(cachedProvider, nullptr);
    void* positionSink = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, referencedObject, 0)[4])(
        referencedObject, nullptr);
    void* entryPosition = static_cast<uint8_t*>(entry) + 4;
    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, positionSink, 0)[5])(
        positionSink, nullptr, entryPosition);

    // orientationSink = GetReferencedObject(provider)->vtbl[4]();
    void* referencedObjectAgain = GetReferencedObject(cachedProvider, nullptr);
    void* orientationSink = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, referencedObjectAgain, 0)[4])(
        referencedObjectAgain, nullptr);

    // angles (degrees) = entry's angles (radians) * 57.29578
    float anglesDegrees[3];
    anglesDegrees[2] = 0.0f;
    anglesDegrees[0] = CSF_AT(float, entry, 0x10) * g_DegreesPerRadian;
    anglesDegrees[1] = CSF_AT(float, entry, 0x14) * g_DegreesPerRadian;
    ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, orientationSink, 0)[19])(
        orientationSink, nullptr, anglesDegrees);

    ObjectProvider_CallWithDefaultOptions(cachedProvider, nullptr);

    // One 0x30-byte record at ebp-0x3C that 0x4F6E90 fills (index, two handles, weight, 4 more dwords); the
    // original then passes the handles inside it and the weight it wrote. A lone uint32 here let the callee
    // overwrite the frame (saved ebp = 0.3f: the FR01 crash).
    OrderRecord rec;
    rec.index = -1;
    rec.source.vtbl = CSF_VTBL(0x0082D448);
    rec.source.field_04 = -1;
    rec.source.field_08 = -1;
    rec.target.vtbl = CSF_VTBL(0x0082D448);
    rec.target.field_04 = -1;
    rec.target.field_08 = -1;
    rec.weight = 0.0f;
    rec.extra[0] = rec.extra[1] = rec.extra[2] = rec.extra[3] = 0;

    if (ObjectProvider_TryFindIndexedEntry(cachedProvider, nullptr, handleIndex, reinterpret_cast<uint32_t*>(&rec))) {
        return static_cast<char>(ObjectProvider_ConfigureOrderTarget(
            cachedProvider, nullptr, &rec.source, &rec.target, rec.weight, nullptr, nullptr));
    }
    return static_cast<char>(ObjectProvider_SetOrderState(cachedProvider, nullptr, 3));
}

HOOK(0x00462580, Scene_SyncEntityTransformAndIssueOrder, "auto");

