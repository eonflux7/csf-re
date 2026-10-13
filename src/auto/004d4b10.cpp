#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00833908*, __fastcall, C_0085e7b8_LookupStateEntry, 0x004E9540, (C_0085e7b8* self, void* , int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static C_00833908* __fastcall AiGiveObject_ResolveStateAtObjectPosition(
    C_008342e8** self, void* /*edx*/)
{
    alignas(8) uint8_t positionRecord[0x1C];

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const int stateId = CSF_AT(int, globalState, 0x181C);
    CSF_AT(int, positionRecord, 0x18) = stateId;

    C_008342e8* object = CSF_AT(C_008342e8*, self, 0x14);
    void* attachedObject = reinterpret_cast<void*>(object->field_002c);
    const int objectId = CSF_AT(int, attachedObject, 0xC0);

    C_0085e7b8* stateManager =
        reinterpret_cast<C_0085e7b8*>(GetValue8B0554());
    C_00833908* entry = C_0085e7b8_LookupStateEntry(
        stateManager, nullptr, -1, objectId, 0, stateId);
    if (!entry)
        return entry;

    object = CSF_AT(C_008342e8*, self, 0x14);
    CSF_AT(int, positionRecord, 0x00) = 0;
    CSF_AT(int, positionRecord, 0x04) = 0;
    CSF_AT(int, positionRecord, 0x08) = 0;
    CSF_AT(int, positionRecord, 0x0C) = 0;
    CSF_AT(float, positionRecord, 0x10) = 0.0f;
    CSF_AT(int, positionRecord, 0x14) = 0;

    int* position = GetObjectPositionVector(object, nullptr);
    CSF_AT(int, positionRecord, 0x00) = position[0];
    object = CSF_AT(C_008342e8*, self, 0x14);
    CSF_AT(int, positionRecord, 0x04) = position[1];
    CSF_AT(int, positionRecord, 0x08) = position[2];
    CSF_AT(float, positionRecord, 0x10) =
        static_cast<float>(C_008342e8_ComputeResolvedObjectScalar(object, nullptr));

    return reinterpret_cast<C_00833908*(__fastcall*)(void*, void*, int*)>(
        reinterpret_cast<void**>(entry->field_000c)[0x8 / 4])(
            &entry->field_000c, nullptr,
            reinterpret_cast<int*>(positionRecord));
}

HOOK(0x004D4B10, AiGiveObject_ResolveStateAtObjectPosition, "auto");
