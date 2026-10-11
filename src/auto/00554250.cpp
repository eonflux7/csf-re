#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SpawnDescriptor_Finish, 0x0040ECF0, ());
CSF_FUNC(int, __fastcall, CScene_AddObject, 0x00453420, (CScene* self, void* , C_00867260* object));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00833908*, __fastcall, S_004e9090_ResolveObject, 0x004EEEF0, (S_004e9090* self, void* , int nameKey, uint32_t param, char altMode));
CSF_FUNC(uint32_t*, __fastcall, SpawnPoints_GetTeamTransform, 0x00553C00, (void* self, void* edx, uint32_t* a2, int a3, uint32_t* a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void*, __fastcall, SpawnRegistry_FindEntry, 0x0073C050, (void* self, void* edx, int arg0, int a2));
CSF_FUNC(int, __fastcall, SceneObject_AttachSpawnEntry, 0x0073F730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SpawnDescriptor_Initialize, 0x00749A60, (void* self, void* edx));

CSF_GLOBAL(float, g_SpawnAngleScale, 0x00844628);
CSF_GLOBAL(std::int32_t, g_NextUniqueId, 0x008A669C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_00833908* __fastcall S_00553680_CreateTeamSpawnPoints(
    S_00553680* self, void* /*edx*/)
{
    C_00833908* result = nullptr;

    for (int team = 0; team < 2; ++team)
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_558150());
        result = reinterpret_cast<C_00833908*>(settings);

        const int nameKey = team
            ? CSF_AT(int, settings, 0x1BC)
            : CSF_AT(int, settings, 0x1B8);
        if (nameKey == -1)
            continue;

        auto* resolver = reinterpret_cast<S_004e9090*>(GetValue8B0554());
        C_00833908* object = S_004e9090_ResolveObject(
            resolver, nullptr, nameKey, 0xFFFFFFFF, 0);
        result = object;
        if (!object)
            continue;

        const std::uint32_t uniqueId =
            static_cast<std::uint32_t>(g_NextUniqueId);
        g_NextUniqueId = static_cast<std::int32_t>(uniqueId + 1);
        object->field_0020 = uniqueId;
        object->field_0024 = 0xFFFFFFFF;

        std::uint32_t flags[3] = {1, 1, 0};
        void* component = reinterpret_cast<std::uint8_t*>(object) + 4;
        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, component, 0)[0x48 / 4])(
                component, nullptr, flags);

        C_008310b8 descriptor{};
        SpawnDescriptor_Initialize(&descriptor, nullptr);
        reinterpret_cast<std::uint8_t*>(&descriptor)[0] =
            static_cast<std::uint8_t>((team != 0) + 1);
        descriptor.field_0004 = nameKey;

        void* registry = reinterpret_cast<void*>(GetGlobalDword_73C020());
        void* entry = SpawnRegistry_FindEntry(
            registry, nullptr, 14, reinterpret_cast<int>(&descriptor));
        result = reinterpret_cast<C_00833908*>(entry);

        if (entry)
        {
            SceneObject_AttachSpawnEntry(
                reinterpret_cast<std::uint8_t*>(object) + 0x14,
                nullptr, reinterpret_cast<int>(entry));
            CScene_AddObject(
                reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                reinterpret_cast<C_00867260*>(object));

            if (team)
            {
                self->field_1414 = object->field_0020;
                self->field_1418 = object->field_0024;
            }
            else
            {
                self->field_1408 = object->field_0020;
                self->field_140c = object->field_0024;
            }

            const int teamIndex = CSF_AT(std::int8_t, entry, -0x30);
            std::uint32_t position[3] = {};
            std::uint32_t angleOutput[2] = {0, 0};
            SpawnPoints_GetTeamTransform(
                self, nullptr, position, teamIndex, angleOutput);
            SceneLoader_ConfigureFocusEntry(
                object, nullptr, reinterpret_cast<int*>(position));

            float angle;
            std::memcpy(&angle, angleOutput, sizeof(angle));
            angle *= g_SpawnAngleScale;

            int payload;
            std::memcpy(&payload, &angle, sizeof(payload));
            result = reinterpret_cast<C_00833908*>(
                DispatchSceneObjectWithPayload(object, nullptr, payload));
        }

        SpawnDescriptor_Finish();
    }

    return result;
}

HOOK(0x00554250, S_00553680_CreateTeamSpawnPoints, "auto");
