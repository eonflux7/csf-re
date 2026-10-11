#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishSpawnDescriptor, 0x0040ECF0, ());
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(char, __fastcall, CScene_SetFocusEntry, 0x0044FCE0, (CScene* self, void* , int requestedEntry));
CSF_FUNC(int, __fastcall, CScene_AddObject, 0x00453420, (CScene* self, void* , C_00867260* object));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(int, __fastcall, C_00495140_ForwardInnerObjectVirtualCall, 0x00495140, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00833908*, __fastcall, S_004e9090_ResolveObject, 0x004EEEF0, (S_004e9090* self, void* , int nameKey, uint32_t param, char altMode));
CSF_FUNC(char*, __fastcall, ObjectProvider_GetCurrentEntry, 0x004F4E90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetProviderFrameState, 0x004F4EA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetProviderPosition, 0x004F4EB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SetProviderPosition, 0x004F4EF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharActionFsm_GetProviderObject, 0x004F4F30, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SetProviderSpawnState, 0x004F5610, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void, __fastcall, GlobalValue4FBDE0_SetToggleMode, 0x004FC210, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, S_00552b80_FindPlayerEntry, 0x00552B80, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00552b80_GetSpawnTransform, 0x00553CF0, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4));
CSF_FUNC(char, __fastcall, S_00552b80_RefreshSpawnState, 0x005543A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void*, __fastcall, MpSpawnPoints_CreatePlayerEntry, 0x0073C050, (void* self, void* edx, int arg0, int a2));
CSF_FUNC(char, __stdcall, MpSpawnPoints_RegisterPlayerEntry, 0x0073CC70, (int a1));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetPlayerEntryId, 0x0073D0F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_AssignPlayerReference, 0x0073F730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ClearPlayerReference, 0x0073F750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_RestorePlayerState, 0x00740B90, (void* self, void* edx, char a2, int a3));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SavePlayerState, 0x00740DE0, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, MpSpawnPoints_SetPlayerSpawnMode, 0x00740F40, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, MpSpawnPoints_CompleteSpawn, 0x00743940, (int a1));
CSF_FUNC(void, __fastcall, MpSpawnPoints_SetDescriptorName, 0x00749A80, (void* self, void* edx, uint16_t* a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_InitializeDescriptor, 0x00749AC0, (void* self, void* edx));

CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);
CSF_GLOBAL(float, g_SpawnRadiansToDegrees, 0x00844628);
CSF_GLOBAL(std::int32_t, g_NextUniqueId, 0x008A669C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet also identifies the lookup method address as a vtable reference.
static void** const g_SpawnLookupTable = CSF_VTBL(0x00552B80);

static int __fastcall S_00552b80_SpawnPlayer(
    S_00552b80* self, void* /*edx*/, int team, int spawnMode, char moveCamera)
{
    int variant = 0;
    int position[3] = {};
    uint32_t savedFrame[3] = {};
    uint32_t savedPosition[3] = {};
    float orientation[3] = {};
    float headingDegrees = 0.0f;
    uint32_t angleScratch = 0;
    uint32_t savedState[3] = {0xFFFFFFFF, 0, 0};
    uint32_t savedStateArgument = 0;
    uint8_t savedFlag = 0;
    float cameraPosition[4] = {};
    uint32_t* framePayload = nullptr;

    if (!self->field_0c8c)
    {
        S_00552b80_GetSpawnTransform(
            self, nullptr, reinterpret_cast<uint32_t*>(position),
            reinterpret_cast<uint32_t*>(&orientation[2]),
            reinterpret_cast<uint32_t*>(&orientation[1]));
    }
    else
    {
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(GetFirstElement(
                reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr)), nullptr);
        auto* frame = reinterpret_cast<uint32_t*>(GetProviderFrameState(provider, nullptr));
        savedFrame[0] = frame[0];
        savedFrame[1] = frame[1];
        savedFrame[2] = frame[2];

        auto* providerPosition = reinterpret_cast<uint32_t*>(
            MpSpawnPoints_GetProviderPosition(provider, nullptr));
        savedPosition[0] = providerPosition[0];
        savedPosition[1] = providerPosition[1];
        savedPosition[2] = providerPosition[2];

        auto* objectPosition = reinterpret_cast<int*>(
            GetObjectPositionVector(reinterpret_cast<void*>(self->field_0c8c), nullptr));
        position[0] = objectPosition[0];
        position[1] = objectPosition[1];
        position[2] = objectPosition[2];

        auto* object = reinterpret_cast<C_00833908*>(self->field_0c8c);
        auto* angles = reinterpret_cast<float*(__fastcall*)(void*, void*, uint32_t*)>(
            object->vtbl[0x38 / 4])(object, nullptr, &angleScratch);
        orientation[1] = angles[0];
        orientation[2] = angles[1];

        void* state = CSF_AT(void*, reinterpret_cast<void*>(self->field_0c8c), 0x3E8);
        savedFlag = CSF_AT(uint8_t, state, 4);
        savedState[2] = CSF_AT(uint32_t, state, 0);

        void* player = self->field_0c90
            ? reinterpret_cast<void*>(self->field_0c90 - 1712) : nullptr;
        MpSpawnPoints_SavePlayerState(player, nullptr, savedState, &savedStateArgument);
        self->field_0c90 = S_00552b80_FindPlayerEntry(self, nullptr);
        if (spawnMode >= 0)
            variant = spawnMode > 1 && spawnMode == 2;
    }

    auto* spawnNames = reinterpret_cast<int*>(GetGlobalValue_558150());
    int objectName = spawnNames[variant + 2 + team + 2 * (variant + 2)];
    spawnNames = reinterpret_cast<int*>(GetGlobalValue_558150());
    savedState[1] = spawnNames[variant + 4 + team + 2 * (variant + 4)];

    self->field_0c8c = reinterpret_cast<uint32_t>(S_004e9090_ResolveObject(
        reinterpret_cast<S_004e9090*>(GetValue8B0554()), nullptr,
        objectName, 0xFFFFFFFF, 0));

    int uniqueId = g_NextUniqueId++;
    auto* object = reinterpret_cast<C_00833908*>(self->field_0c8c);
    object->field_0020 = uniqueId;
    object->field_0024 = 0xFFFFFFFF;
    int dispatched = C_0082dd78_DispatchReturnedObject(
        reinterpret_cast<C_0082dd78*>(
            reinterpret_cast<C_00833908*>(self->field_0c8c)->field_004c), nullptr);
    CSF_AT(int, reinterpret_cast<void*>(dispatched), 4) = 2;

    CScene_AddObject(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                    reinterpret_cast<C_00867260*>(self->field_0c8c));
    CScene_SetFocusEntry(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr, self->field_0c8c);
    reinterpret_cast<C_00833908*>(self->field_0c8c)->vSlot50(position);

    headingDegrees = orientation[1] * g_SpawnRadiansToDegrees;
    const float initialHeadingDegrees = headingDegrees;
    int headingPayload;
    std::memcpy(&headingPayload, &headingDegrees, sizeof(headingPayload));
    DispatchSceneObjectWithPayload(
        reinterpret_cast<void*>(self->field_0c8c), nullptr, headingPayload);

    object = reinterpret_cast<C_00833908*>(self->field_0c8c);
    void* inner = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        object->vtbl[0xA0 / 4])(object, nullptr);
    void* transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, inner, 0)[0x18 / 4])(inner, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, transform, 0)[0x34 / 4])(transform, nullptr, initialHeadingDegrees);

    if (moveCamera)
    {
        reinterpret_cast<C_00833908*>(self->field_0c8c)->vSlot34(
            reinterpret_cast<uint32_t*>(cameraPosition));
        cameraPosition[1] += g_QuarterTurnConstant;
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(GetFirstElement(
                reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr)), nullptr);
        ObjectProvider_SetUpdateMode(provider, nullptr, 5);

        provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(GetFirstElement(
                reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr)), nullptr);
        void* referenced = GetReferencedObject(provider, nullptr);
        transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, transform, 0)[0x14 / 4])(transform, nullptr, cameraPosition);

        provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(GetFirstElement(
                reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr)), nullptr);
        referenced = GetReferencedObject(provider, nullptr);
        transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, transform, 0)[0x34 / 4])(transform, nullptr, initialHeadingDegrees);
    }

    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(GetFirstElement(
            reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr)), nullptr);
    if (CSF_AT(int, ObjectProvider_GetCurrentEntry(provider, nullptr), 8) >= 0)
    {
        object = reinterpret_cast<C_00833908*>(self->field_0c8c);
        if (CSF_AT(uint32_t, ObjectProvider_GetCurrentEntry(provider, nullptr), 4)
            != object->field_0020)
        {
            if (self->field_13d0 &&
                reinterpret_cast<C_00833908*>(self->field_0c8c)->field_0078)
            {
                ObjectProvider_SetUpdateMode(provider, nullptr, 7);
                framePayload = savedFrame;
                CharActionFsm_GetProviderObject(provider, nullptr);
                MpSpawnPoints_SetProviderPosition(
                    provider, nullptr, reinterpret_cast<int>(savedPosition));
            }
            else
            {
                cameraPosition[0] = 0.0f;
                cameraPosition[1] = 0.0f;
                cameraPosition[2] = 0.0f;
                S_00552b80_GetSpawnTransform(
                    self, nullptr, reinterpret_cast<uint32_t*>(cameraPosition),
                    reinterpret_cast<uint32_t*>(&orientation[2]),
                    reinterpret_cast<uint32_t*>(&headingDegrees));
                void* referenced = GetReferencedObject(provider, nullptr);
                transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);
                orientation[2] = cameraPosition[2];
                orientation[0] = cameraPosition[0];
                orientation[1] =
                    CSF_AT(float, reinterpret_cast<void*>(self->field_0c8c), 0x1D4)
                    + g_QuarterTurnConstant + cameraPosition[1];
                reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                    CSF_AT(void**, transform, 0)[0x14 / 4])(transform, nullptr, orientation);

                referenced = GetReferencedObject(provider, nullptr);
                transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, transform, 0)[0x30 / 4])(transform, nullptr, 0);

                referenced = GetReferencedObject(provider, nullptr);
                transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);
                reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                    CSF_AT(void**, transform, 0)[0x34 / 4])(
                        transform, nullptr, headingDegrees * g_SpawnRadiansToDegrees);
                ObjectProvider_SetUpdateMode(provider, nullptr, 5);
                int global = GetGlobalValue_4FBDE0();
                GlobalValue4FBDE0_SetToggleMode(reinterpret_cast<void*>(global), nullptr, 0);
            }
        }
    }

    if (self->field_0c90)
    {
        if (self->field_0c98)
            MpSpawnPoints_ClearPlayerReference(
                reinterpret_cast<uint8_t*>(self) + 0xC94, nullptr);
        void* player = self->field_0c90
            ? reinterpret_cast<void*>(self->field_0c90 - 1712) : nullptr;
        CSF_AT(uint32_t, player, 1372) = savedState[1];
        CSF_AT(uint8_t, player, 1112) = static_cast<uint8_t>(team);
    }
    else
    {
        alignas(4) uint8_t descriptor[0x210] = {};
        MpSpawnPoints_InitializeDescriptor(descriptor, nullptr);
        void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
        uint32_t capacity = CSF_AT(uint32_t, manager, 36);
        auto* nameObject = reinterpret_cast<uint8_t*>(manager) + 12;
        uint16_t* name = capacity < 8
            ? reinterpret_cast<uint16_t*>(nameObject + 4)
            : CSF_AT(uint16_t*, nameObject, 4);
        MpSpawnPoints_SetDescriptorName(descriptor, nullptr, name);
        CSF_AT(uint32_t, descriptor, 0x104) = savedState[1];
        manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
        CSF_AT(int, descriptor, 0x108) =
            MpSpawnPoints_GetPlayerEntryId(manager, nullptr);
        descriptor[0] = static_cast<uint8_t>(team);
        manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
        void* entry = MpSpawnPoints_CreatePlayerEntry(
            manager, nullptr, 0, reinterpret_cast<int>(descriptor));
        self->field_0c90 = reinterpret_cast<uint32_t>(entry);
        GetGlobalDword_73C020();
        MpSpawnPoints_RegisterPlayerEntry(reinterpret_cast<int>(entry));
        MpSpawnPoints_FinishSpawnDescriptor();
    }

    if (self->field_0c90)
    {
        void* innerObject = reinterpret_cast<void*>(
            reinterpret_cast<C_00833908*>(self->field_0c8c)->field_0038);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, uint32_t*)>(
            CSF_AT(void**, innerObject, 0)[164 / 4])(
                innerObject, nullptr, 0, framePayload);
        reinterpret_cast<C_00833908*>(self->field_0c8c)->field_0078 = spawnMode != 1;
        if (moveCamera)
        {
            void* currentProvider = GetCachedObjectProvider(
                reinterpret_cast<void*>(GetFirstElement(
                    reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr)), nullptr);
            MpSpawnPoints_SetProviderSpawnState(currentProvider, nullptr, 0);
        }
        MpSpawnPoints_AssignPlayerReference(
            reinterpret_cast<uint8_t*>(self) + 0xC94, nullptr, self->field_0c90);
        reinterpret_cast<C_00833908*>(self->field_0c8c)->vSlot6C(0);
        C_00495140_ForwardInnerObjectVirtualCall(
            reinterpret_cast<void*>(self->field_0c8c), nullptr);
        void* player = self->field_0c90
            ? reinterpret_cast<void*>(self->field_0c90 - 1712) : nullptr;
        MpSpawnPoints_SetPlayerSpawnMode(player, nullptr, spawnMode);
    }

    if (savedFlag)
    {
        void* state = CSF_AT(void*, reinterpret_cast<void*>(self->field_0c8c), 0x3E8);
        CSF_AT(uint8_t, state, 4) = savedFlag;
        state = CSF_AT(void*, reinterpret_cast<void*>(self->field_0c8c), 0x3E8);
        CSF_AT(uint32_t, state, 0) = savedState[2];
        void* player = self->field_0c90
            ? reinterpret_cast<void*>(self->field_0c90 - 1712) : nullptr;
        MpSpawnPoints_RestorePlayerState(
            player, nullptr, static_cast<char>(savedState[0]), savedStateArgument);
    }

    S_00552b80_RefreshSpawnState(self, nullptr);
    int result = MpSpawnPoints_CompleteSpawn(self->field_1484);
    self->field_148c = 0;
    return result;
}

HOOK(0x00554700, S_00552b80_SpawnPlayer, "auto");
