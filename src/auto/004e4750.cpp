#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, CreateReplacementObject, 0x00441F10, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __fastcall, C_008310f8_Initialize, 0x00446550, (C_008310f8* self, void* , int owner));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __fastcall, CScene_AddObject, 0x00453420, (CScene* self, void* , C_00867260* object));
CSF_FUNC(void, __fastcall, Game_SetMappedValue, 0x00455DF0, (void* self, void* , const void* key, const void* value));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, GetRelatedObjectAndClearFlag, 0x0048E6F0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static void __fastcall C_0083e7d8_UpdateReplacementTimer(
    C_0083e7d8* self, void* /*edx*/, float elapsed)
{
    if (self->field_0050 > g_fZero)
    {
        self->field_0050 = self->field_0050 - elapsed;
        return;
    }

    void* gameState = reinterpret_cast<void*>(GetValue8B0554());
    void* definitions = CSF_AT(void*, gameState, 0x1FC8);
    void* owner = reinterpret_cast<void*>(self->field_000c);
    void* ownerData = CSF_AT(void*, owner, 0x2C);
    int definitionId = CSF_AT(int, ownerData, 0xC4);
    int index = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, definitions, 0)[0x10 / 4])(
            definitions, nullptr, definitionId);

    if (index != -1)
    {
        gameState = reinterpret_cast<void*>(GetValue8B0554());
        definitions = CSF_AT(void*, gameState, 0x1FC8);
        void* definition = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, definitions, 0)[0x8 / 4])(
                definitions, nullptr, index);
        int objectId = CSF_AT(int, definition, 0);
        int objectType = CSF_AT(int, definition, 0xB4);
        void* factory = reinterpret_cast<void*>(GetValue_8afc80());
        C_00834050* replacement = reinterpret_cast<C_00834050*>(
            CreateReplacementObject(factory, nullptr, objectType, objectId, 0));

        int* position = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0054), nullptr);
        SceneLoader_ConfigureFocusEntry(replacement, nullptr, position);

        C_008342e8* oldObject =
            reinterpret_cast<C_008342e8*>(self->field_0054);
        void* oldComponent =
            reinterpret_cast<void*>(oldObject->vGetField016c());
        void* oldState = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, oldComponent, 0)[0x18 / 4])(
                oldComponent, nullptr);

        void* newComponent =
            reinterpret_cast<void*>(replacement->vGetField016c());
        void* newState = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, newComponent, 0)[0x18 / 4])(
                newComponent, nullptr);
        void** newStateVtable = CSF_AT(void**, newState, 0);
        int state = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, oldState, 0)[0x48 / 4])(oldState, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            newStateVtable[0x4C / 4])(newState, nullptr, state);

        reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
            replacement->vtbl[0x8C / 4])(
                replacement, nullptr, 0, 0, -1, 0);
        CScene_AddObject(reinterpret_cast<CScene*>(g_pCurrentScene),
                         nullptr, reinterpret_cast<C_00867260*>(replacement));
        Game_SetMappedValue(
            g_pCurrentScene, nullptr,
            reinterpret_cast<uint8_t*>(self->field_000c) + 0x1C,
            &replacement->field_001c);

        GetRelatedObjectAndClearFlag(
            reinterpret_cast<void*>(self->field_0054), nullptr);
        oldObject = reinterpret_cast<C_008342e8*>(self->field_0054);
        oldComponent = reinterpret_cast<void*>(oldObject->vGetField016c());
        void* related = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, oldComponent, 0)[0x78 / 4])(
                oldComponent, nullptr);
        if (related &&
            reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, related, 0)[0x28 / 4])(related, nullptr))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, related, 0)[0x24 / 4])(related, nullptr);
        }

        alignas(8) uint8_t recordBytes[0x64];
        C_008310f8& record =
            *reinterpret_cast<C_008310f8*>(recordBytes);
        C_008310f8_Initialize(&record, nullptr, 14);
        record.field_0024 = 4;
        uint32_t* recordPosition = reinterpret_cast<uint32_t*>(
            GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0054), nullptr));
        record.field_0034 = recordPosition[0];
        uint32_t positionY = recordPosition[1];
        uint32_t recordObject = self->field_0054;
        record.field_0038 = positionY;
        record.field_003c = recordPosition[2];
        record.field_0040 = recordObject;
        record.field_0028 = 1084227584;
        record.field_0044 = 0;
        record.field_0054 = 0;
        record.field_0058 = 0;
        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1);

        void* notificationTarget =
            reinterpret_cast<void*>(self->field_0038);
        if (notificationTarget)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, notificationTarget, 0)[0x3C / 4])(
                    notificationTarget, nullptr, 1161527296);
        }
    }

    C_00833908* oldObject =
        reinterpret_cast<C_00833908*>(self->field_0054);
    if (oldObject)
    {
        CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            reinterpret_cast<int>(&oldObject->field_001c));
        self->field_0054 = 0;
    }
    self->field_004c = 0;
}

HOOK(0x004E4750, C_0083e7d8_UpdateReplacementTimer, "auto");
