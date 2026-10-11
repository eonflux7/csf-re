#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, InitDefaultFields, 0x00461940, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00833908*, __fastcall, C_0085e7b8_LookupStateEntry, 0x004E9540, (C_0085e7b8* self, void* , int a2, int a3, int a4, int a5));
CSF_FUNC(void*, __stdcall, MpSpawnPoints_ProcessSpawnRecord, 0x005536C0, (int a1));
CSF_FUNC(uint32_t*, __fastcall, MpSpawnPoints_ComputeTeamSpawnTransform, 0x00553C00, (void* self, void* edx, uint32_t* a2, int a3, uint32_t* a4));
CSF_FUNC(int, __fastcall, MpSpawnPoints_RegisterBombRecord, 0x00556880, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetRecordOwnerId, 0x0073CD90, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, MpSpawnPoints_GetNextPendingRecord, 0x0073DCE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_FindReferencedRecord, 0x0073E420, (void* self, void* edx, int key));
CSF_FUNC(void*, __fastcall, MpSpawnPoints_ConstructRecordNode, 0x0073F700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_AssignRecord, 0x0073F730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SpawnRadiansToDegrees, 0x00844628);

struct SpawnEventPayload {
    uint32_t fields[6];
    int eventCode;
    void** vtbl;
    int sourceId;
    int sourceState;
};

struct SpawnSourceIdentifier {
    void** vtbl;
    int id;
    int state;
};

static int __fastcall MpSpawnPoints_ProcessPendingEntries(void* self, void* /*edx*/)
{
    int result = GetGlobalDword_73C020();
    if (!result)
        return result;

    result = GetGlobalDword_73C020();
    if (CSF_AT(int, reinterpret_cast<void*>(result), 0x98) != 6)
        return result;

    int deferredType = -1;
    for (;;) {
        const int excludedType = deferredType;
        void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
        uint32_t* record = MpSpawnPoints_GetNextPendingRecord(manager, nullptr, excludedType);
        deferredType = -1;
        if (!record)
            return 0;

        switch (CSF_AT(uint32_t, record, 0x0C)) {
        case 0:
            MpSpawnPoints_ProcessSpawnRecord(reinterpret_cast<int>(record));
            break;

        case 1: {
            void* storage = Mem_Alloc(8);
            void* node = storage
                ? MpSpawnPoints_ConstructRecordNode(storage, nullptr)
                : nullptr;
            CSF_AT(void*, self, 0xC88) = node;
            MpSpawnPoints_AssignRecord(node, nullptr, reinterpret_cast<int>(record));
            break;
        }

        case 4:
            if (CSF_AT(float, self, 0x147C) > g_fZero) {
                deferredType = 4;
            } else {
                void* storage = Mem_Alloc(8);
                if (storage) {
                    uint32_t node = reinterpret_cast<uint32_t>(
                        MpSpawnPoints_ConstructRecordNode(storage, nullptr));
                    if (node) {
                        void* owner = reinterpret_cast<void*>(GetGlobalDword_73C020());
                        CSF_AT(int, record, -0x04) =
                            MpSpawnPoints_GetRecordOwnerId(owner, nullptr);
                        MpSpawnPoints_AssignRecord(
                            reinterpret_cast<void*>(node), nullptr,
                            reinterpret_cast<int>(record));
                        MpSpawnPoints_RegisterBombRecord(self, nullptr, &node);
                    }
                }
            }
            break;

        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 0xA:
        case 0xB: {
            const int key = CSF_AT(int, record, -0x0C);
            const int type = CSF_AT(uint16_t, record, -0x28);
            auto* registry = reinterpret_cast<C_0085e7b8*>(GetValue8B0554());
            C_00833908* entry =
                C_0085e7b8_LookupStateEntry(registry, nullptr, type, key, 1, 0);
            if (!entry)
                break;

            MpSpawnPoints_AssignRecord(&entry->field_0014, nullptr,
                                      reinterpret_cast<int>(record));
            void* related = reinterpret_cast<void*>(entry->vGetField016c());
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, related, 0)[0x3C / 4])(related, nullptr, 1);

            SpawnEventPayload payload{};
            InitDefaultFields(&payload, nullptr);
            const int eventCode = CSF_AT(int, record, -0x2C);
            const int referenceKey = CSF_AT(int, record, -0x08);
            payload.eventCode = eventCode;
            void* owner = reinterpret_cast<void*>(GetGlobalDword_73C020());
            int reference = MpSpawnPoints_FindReferencedRecord(owner, nullptr, referenceKey);
            int value = reference
                ? GetValueIfNotFlagged(reinterpret_cast<void*>(reference), nullptr)
                : 0;

            SpawnSourceIdentifier fallback{};
            void* source;
            const bool useFallback = !value || value == 20;
            if (useFallback) {
                fallback.id = -1;
                fallback.state = -1;
                fallback.vtbl = CSF_VTBL(0x0082D448);
                source = &fallback;
            } else {
                source = reinterpret_cast<uint8_t*>(value) - 20 + 28;
            }
            payload.sourceId = CSF_AT(int, source, 0x04);
            payload.sourceState = CSF_AT(int, source, 0x08);
            if (useFallback)
                fallback.vtbl = CSF_VTBL(0x0082D410);

            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                reinterpret_cast<void**>(entry->field_000c)[0x08 / 4])(
                    &entry->field_000c, nullptr, &payload);
            payload.vtbl = CSF_VTBL(0x0082D410);
            break;
        }

        case 0xC: {
            const int key = CSF_AT(int, record, -0x18);
            const int type = CSF_AT(uint16_t, record, -0x32);
            auto* registry = reinterpret_cast<C_0085e7b8*>(GetValue8B0554());
            C_00833908* entry =
                C_0085e7b8_LookupStateEntry(registry, nullptr, type, key, 1, 0);
            if (!entry)
                break;

            MpSpawnPoints_AssignRecord(&entry->field_0014, nullptr,
                                      reinterpret_cast<int>(record));
            void* related = reinterpret_cast<void*>(entry->vGetField016c());
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, related, 0)[0x3C / 4])(related, nullptr, 1);

            if (CSF_AT(uint16_t, record, -0x32) == 11) {
                SpawnEventPayload payload{};
                InitDefaultFields(&payload, nullptr);
                const int referenceKey = CSF_AT(int, record, -0x14);
                void* owner = reinterpret_cast<void*>(GetGlobalDword_73C020());
                int reference =
                    MpSpawnPoints_FindReferencedRecord(owner, nullptr, referenceKey);
                int value = reference
                    ? GetValueIfNotFlagged(reinterpret_cast<void*>(reference), nullptr)
                    : 0;

                SpawnSourceIdentifier fallback{};
                void* source;
                const bool useFallback = !value || value == 20;
                if (useFallback) {
                    fallback.id = -1;
                    fallback.state = -1;
                    fallback.vtbl = CSF_VTBL(0x0082D448);
                    source = &fallback;
                } else {
                    source = reinterpret_cast<uint8_t*>(value) - 20 + 28;
                }
                payload.sourceId = CSF_AT(int, source, 0x04);
                payload.sourceState = CSF_AT(int, source, 0x08);
                if (useFallback)
                    fallback.vtbl = CSF_VTBL(0x0082D410);

                payload.eventCode = CSF_AT(uint16_t, record, -0x34);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    reinterpret_cast<void**>(entry->field_000c)[0x08 / 4])(
                        &entry->field_000c, nullptr, &payload);
                payload.vtbl = CSF_VTBL(0x0082D410);
            } else {
                uint32_t payload[6]{};
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    reinterpret_cast<void**>(entry->field_000c)[0x08 / 4])(
                        &entry->field_000c, nullptr, payload);
            }
            break;
        }

        case 0xD: {
            const int key = CSF_AT(int, record, -0x58);
            auto* registry = reinterpret_cast<C_0085e7b8*>(GetValue8B0554());
            C_00833908* entry =
                C_0085e7b8_LookupStateEntry(registry, nullptr, 31, key, 1, 0);
            if (entry)
                MpSpawnPoints_AssignRecord(&entry->field_0014, nullptr,
                                          reinterpret_cast<int>(record));
            break;
        }

        case 0xE: {
            const int key = CSF_AT(int, record, -0x2C);
            auto* registry = reinterpret_cast<C_0085e7b8*>(GetValue8B0554());
            auto* entry = static_cast<C_00834050*>(
                C_0085e7b8_LookupStateEntry(registry, nullptr, 32, key, 1, 0));
            if (!entry)
                break;

            MpSpawnPoints_AssignRecord(&entry->field_0014, nullptr,
                                      reinterpret_cast<int>(record));
            const int team = CSF_AT(int8_t, record, -0x30);
            int position[3]{};
            uint32_t headingStorage[4]{};
            MpSpawnPoints_ComputeTeamSpawnTransform(
                self, nullptr, reinterpret_cast<uint32_t*>(position), team,
                headingStorage);
            SceneLoader_ConfigureFocusEntry(entry, nullptr, position);

            float heading;
            std::memcpy(&heading, headingStorage, sizeof(heading));
            const float degrees = heading * g_SpawnRadiansToDegrees;
            int anglePayload;
            std::memcpy(&anglePayload, &degrees, sizeof(anglePayload));
            DispatchSceneObjectWithPayload(entry, nullptr, anglePayload);

            if (CSF_AT(uint8_t, record, -0x30) == 1) {
                CSF_AT(uint32_t, self, 0x1408) = entry->field_0020;
                CSF_AT(uint32_t, self, 0x140C) = entry->field_0024;
            } else if (CSF_AT(uint8_t, record, -0x30) == 2) {
                CSF_AT(uint32_t, self, 0x1414) = entry->field_0020;
                CSF_AT(uint32_t, self, 0x1418) = entry->field_0024;
            }

            uint32_t options[9]{1, 1, 0};
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*, float)>(
                reinterpret_cast<void**>(entry->field_0004)[0x48 / 4])(
                    &entry->field_0004, nullptr, options, degrees);
            break;
        }

        default:
            break;
        }
    }
}

HOOK(0x005568C0, MpSpawnPoints_ProcessPendingEntries, "auto");
