#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_ResetSelection, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_SetSelectionMode, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetSession_ProcessSelectedObject, 0x0064D1A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_InitializeObjectReference, 0x00651A60, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SessionScope_Begin, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, NetSession_CopyObjectReference, 0x00651D30, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, SessionFlagBuffer_SetMode, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(std::uint32_t*, __fastcall, NetSession_InitializeSelectionIterator, 0x0073D710, (std::uint32_t* self, void*));
CSF_FUNC(void, __fastcall, NetCogibleDDL_AdvanceStateMachine, 0x00740860, (void* self, void* , float deltaTime));
CSF_FUNC(void*, __cdecl, NetSession_GetSpawnRecordFromReference, 0x0074A9C0, (void* a1));
CSF_FUNC(void, __stdcall, NetDdlObjectManager_HandleSpawnRecordState, 0x0074ACB0, (int a1));

static int NetDdlObjectManager_GetSelectedObject(void* selection)
{
    if (CSF_AT(int, selection, 0x4))
        return CSF_AT(int, selection, 0x0);

    Game_RaiseStatusNotice(0, 0, -1610612729, 0);
    return 0;
}

static void* NetDdlObjectManager_ValidateAndReleaseReference(
    C_00873b14* reference)
{
    void* selectedObject = nullptr;
    void* object = reinterpret_cast<void*>(reference->vtbl);

    if (object)
    {
        void* indexedObject = reinterpret_cast<void*>(
            LookupObjectByIndex(CSF_AT(std::uint32_t, object, 0x2C) >> 22));

        if (((bool(__fastcall*)(void*, void*, int))
                CSF_AT(void**, indexedObject, 0)[0x3C / 4])(
                    indexedObject, nullptr, 1))
        {
            selectedObject = reinterpret_cast<void*>(reference->vtbl);
        }
        else
        {
            Game_RaiseStatusNotice(0, 0, -536674292, 0);
        }
    }
    else
    {
        Game_RaiseStatusNotice(0, 0, -1610416124, 0);
    }

    SpawnPoints_ReleaseEntry(reference, nullptr);
    return selectedObject;
}

static void __fastcall NetDdlObjectManager_Update(
    std::uint8_t* self, void* /*edx*/, float deltaTime)
{
    // Preserve the contiguous iterator/reference storage in the original frame.
    alignas(8) std::uint8_t frameBytes[0x74];
    auto* iterator = reinterpret_cast<std::uint32_t*>(frameBytes);
    void* selection = frameBytes + 0x4;

    auto& spawnReference =
        *reinterpret_cast<C_00873b14*>(frameBytes + 0x24);
    auto& stateThreeTemporary =
        *reinterpret_cast<C_00873b14*>(frameBytes + 0x30);
    auto& resetTemporary =
        *reinterpret_cast<C_00873b14*>(frameBytes + 0x3C);
    auto& spawnTemporary =
        *reinterpret_cast<C_00873b14*>(frameBytes + 0x48);
    auto& stateThreeReference =
        *reinterpret_cast<C_00873b14*>(frameBytes + 0x54);
    auto& resetReference =
        *reinterpret_cast<C_00873b14*>(frameBytes + 0x60);
    auto* scopeValue =
        reinterpret_cast<std::uint32_t*>(frameBytes + 0x6C);

    NetSession_InitializeSelectionIterator(iterator, nullptr);
    if (*iterator)
    {
        NetSession_ResetSelection(iterator, nullptr);
        NetSession_SetSelectionMode(iterator, nullptr, 1);
    }
    else
    {
        *scopeValue = 0;
        SessionScope_Begin(selection, nullptr, scopeValue);
    }

    while (CSF_AT(int, selection, 0x4))
    {
        void* selectedObject = nullptr;
        void* currentObject =
            reinterpret_cast<void*>(CSF_AT(int, selection, 0x0));

        if (!((bool(__fastcall*)(void*, void*))
                  CSF_AT(void**, currentObject, 0)[0x34 / 4])(
                      currentObject, nullptr))
        {
            NetSession_InitializeObjectReference(
                &spawnTemporary, nullptr,
                NetDdlObjectManager_GetSelectedObject(selection));
            NetSession_CopyObjectReference(
                &spawnReference, nullptr,
                reinterpret_cast<int*>(&spawnTemporary));
            SpawnPoints_ReleaseEntry(&spawnTemporary, nullptr);

            void* spawnRecord =
                NetSession_GetSpawnRecordFromReference(&spawnReference);
            if (spawnRecord)
            {
                NetCogibleDDL_AdvanceStateMachine(
                    spawnRecord, nullptr, deltaTime);

                switch (((int(__fastcall*)(void*, void*))
                             CSF_AT(void**, spawnRecord, 0)[0x4 / 4])(
                                 spawnRecord, nullptr))
                {
                case 0:
                case 2:
                    if (*self)
                    {
                        NetSession_InitializeObjectReference(
                            &resetTemporary, nullptr,
                            NetDdlObjectManager_GetSelectedObject(selection));
                        NetSession_CopyObjectReference(
                            &resetReference, nullptr,
                            reinterpret_cast<int*>(&resetTemporary));
                        SpawnPoints_ReleaseEntry(&resetTemporary, nullptr);
                        selectedObject =
                            NetDdlObjectManager_ValidateAndReleaseReference(
                                &resetReference);
                    }
                    else
                    {
                        ((void(__fastcall*)(void*, void*, float))
                             CSF_AT(void**, spawnRecord, 0)[0xC / 4])(
                                 spawnRecord, nullptr, deltaTime);
                    }
                    break;

                case 1:
                    NetDdlObjectManager_HandleSpawnRecordState(
                        reinterpret_cast<int>(spawnRecord));
                    break;

                case 3:
                    NetSession_InitializeObjectReference(
                        &stateThreeTemporary, nullptr,
                        NetDdlObjectManager_GetSelectedObject(selection));
                    NetSession_CopyObjectReference(
                        &stateThreeReference, nullptr,
                        reinterpret_cast<int*>(&stateThreeTemporary));
                    SpawnPoints_ReleaseEntry(&stateThreeTemporary, nullptr);
                    selectedObject =
                        NetDdlObjectManager_ValidateAndReleaseReference(
                            &stateThreeReference);
                    break;

                default:
                    break;
                }
            }

            SpawnPoints_ReleaseEntry(&spawnReference, nullptr);
        }

        NetSession_SetSelectionMode(iterator, nullptr, 0);
        if (selectedObject &&
            static_cast<std::uint8_t>(
                Sub_64B250(selectedObject, nullptr)) != 0)
        {
            NetSession_ProcessSelectedObject(selectedObject, nullptr);
        }
    }

    *self = 0;
    SessionFlagBuffer_SetMode(iterator, nullptr, 0);
    std::uint32_t emptyScope = 0;
    SessionScope_Begin(selection, nullptr, &emptyScope);
    SpawnPoints_ReleaseEntry(selection, nullptr);
}

HOOK(0x0074AE60, NetDdlObjectManager_Update, "auto");
