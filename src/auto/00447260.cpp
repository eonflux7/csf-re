#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t*, __fastcall, InitDrawCommandRecord, 0x00445FE0, (void* self, void* edx, int a2, int a3, uint32_t* a4, int a5, int a6));
CSF_FUNC(uint32_t*, __fastcall, SceneRequest_InitResolvedObjectRecord, 0x00446040, (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5));
CSF_FUNC(uint32_t*, __fastcall, SceneRequest_InitObjectRecord, 0x004460A0, (void* self, void* edx, int a2, int a3, uint32_t* a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, RenderQueuePushRecord, 0x00446D30, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(unsigned int*, __fastcall, SceneRequest_Submit, 0x00446F10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObject_GetDrawState, 0x0049CF10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static unsigned int* __fastcall SceneRequest_ProcessObjectRecord(
    char* self, void* /*edx*/, uint32_t* record)
{
    // The two temporary records share the original 0x28-byte stack area.
    alignas(8) uint8_t recordBytes[0x28];
    auto* requestRecord = reinterpret_cast<uint32_t*>(recordBytes);
    auto* drawRecord = reinterpret_cast<uint32_t*>(recordBytes + 4);

    if (record[1] == 4 &&
        static_cast<int32_t>(record[4]) >= 0 &&
        record[23] != static_cast<uint32_t>(-1))
    {
        void* resolvedObject = reinterpret_cast<void*>(
            CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(record + 2)));

        if (resolvedObject && CSF_AT(uint32_t, resolvedObject, 0x50) == 0)
        {
            void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
            void* objectTable = CSF_AT(void*, globalState, 0xE8);
            void* entry =
                reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, objectTable, 0)[2])(
                        objectTable, nullptr, static_cast<int>(record[23]));
            int objectId = CSF_AT(int, entry, 0x24);

            if (objectId != -1)
            {
                SceneRequest_InitResolvedObjectRecord(
                    requestRecord, nullptr, 9, objectId,
                    reinterpret_cast<int>(
                        static_cast<uint8_t*>(resolvedObject) + 0x1C),
                    record + 13);
                SceneRequest_Submit(
                    self - 4, nullptr, reinterpret_cast<int>(requestRecord));
            }
        }
    }

    unsigned int* result = reinterpret_cast<unsigned int*>(record[1]);
    if (record[1] == 14 || record[1] == 15)
    {
        auto* object = reinterpret_cast<C_008342e8*>(record[16]);
        if (object)
        {
            if (static_cast<uint8_t>(
                    GameConfig_IsSceneObjectRemovable(
                        static_cast<int>(record[9]))) != 0)
            {
                void* drawState =
                    reinterpret_cast<void*>(SceneObject_GetDrawState(object, nullptr));
                int drawParameter6 = CSF_AT(int, drawState, 0x1A0);
                int drawParameter5 = CSF_AT(int, drawState, 0x120);

                InitDrawCommandRecord(
                    drawRecord, nullptr, 9,
                    reinterpret_cast<int>(&object->field_001c),
                    record + 13, drawParameter5, drawParameter6);
                RenderQueuePushRecord(self - 4, nullptr, drawRecord);
            }

            result = reinterpret_cast<unsigned int*>(record[17]);
            if (!result)
            {
                void* globalValue =
                    reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                int requestParameter = CSF_AT(int, globalValue, 0xA7C);

                SceneRequest_InitObjectRecord(
                    requestRecord, nullptr, 7,
                    reinterpret_cast<int>(&object->field_001c),
                    record + 13, requestParameter);
                return SceneRequest_Submit(
                    self - 4, nullptr, reinterpret_cast<int>(requestRecord));
            }
        }
    }

    return result;
}

HOOK(0x00447260, SceneRequest_ProcessObjectRecord, "auto");
