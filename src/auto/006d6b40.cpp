#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxCollisionShapes_FindFirstLiveShapeIndex, 0x006CDF20, (C_00860c88* self, void* , char* a2));
CSF_FUNC(int, __fastcall, GfxCollisionShapes_GetRecordValue, 0x006CDF90, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, CollisionShapes_ApplySetupAndInitEntry, 0x006CEF30, (S_006cee30* self, void* , char* a2));
CSF_FUNC(uint32_t*, __cdecl, GfxTeam_CreatePlayerForReference, 0x006FF490, (void* a2));

static int __fastcall C_00861120_CreatePlayerForShape(
    C_00861120* self, void* /*edx*/, char* shapeName)
{
    if (!self->field_00d9)
        return 0;

    void* globalAccessor = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneLoader = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalAccessor, nullptr));

    auto* collection = reinterpret_cast<C_00860c88*>(
        SceneLoader_GetWorldObjectCollection(sceneLoader, nullptr));
    int shapeIndex = GfxCollisionShapes_FindFirstLiveShapeIndex(
        collection, nullptr, shapeName);

    if (shapeIndex == -1)
    {
        auto* setupCollection = reinterpret_cast<S_006cee30*>(
            SceneLoader_GetWorldObjectCollection(sceneLoader, nullptr));
        shapeIndex = CollisionShapes_ApplySetupAndInitEntry(
            setupCollection, nullptr, shapeName);
    }

    void* recordCollection = reinterpret_cast<void*>(
        SceneLoader_GetWorldObjectCollection(sceneLoader, nullptr));
    void* record = reinterpret_cast<void*>(
        GfxCollisionShapes_GetRecordValue(recordCollection, nullptr, shapeIndex));

    if (!record)
        return 0;

    int recordKind =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, record, 0)[0x10 / 4])(record, nullptr);

    if (recordKind != 1 && *self->vSlot5C() >= self->field_0004)
        return 0;

    uint32_t* player = GfxTeam_CreatePlayerForReference(record);
    if (!player)
        return 0;

    if (self->field_005c >= self->field_0058)
    {
        // Listed by the packet as a vtable reference; the instruction calls it.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x54),
            nullptr, CSF_AT(int, self, 0x60));
    }

    uint32_t index = self->field_005c;
    int* destination = reinterpret_cast<int*>(self->field_0054) + index;
    self->field_005c = index + 1;
    int playerValue = static_cast<int>(reinterpret_cast<intptr_t>(player));
    *destination = playerValue;
    return playerValue;
}

HOOK(0x006D6B40, C_00861120_CreatePlayerForShape, "auto");
