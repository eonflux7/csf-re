#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(void, __fastcall, NetSession_ResetRecordIterator, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_AdvanceRecordIterator, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_ConstructRecordReference, 0x00651A60, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_ReleaseRecordReference, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_AssignRecordReference, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, NetSession_CopyRecordReference, 0x00651D30, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, NetSession_FinishRecordIterator, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, NetSession_ConstructSpawnRecordIterator, 0x0073D710, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, NetSession_GetSpawnRecordFromReference, 0x0074A9C0, (void* a1));

static uint32_t* __fastcall NetSession_GetNextPendingSpawnRecord(
    void* self, void* /*edx*/, int excludedId)
{
    uint32_t* result = nullptr;
    if (CSF_AT(int, self, 0x98) != 6)
        return result;

    // Preserve the complete original frame, including the iterator's
    // adjacent reference storage.
    alignas(8) uint8_t frameBytes[0x44];
    void* iterator = frameBytes;
    void* currentReference = frameBytes + 0x4;
    void* convertedReference = frameBytes + 0x24;
    void* temporaryReference = frameBytes + 0x30;
    uint32_t* emptyReference =
        reinterpret_cast<uint32_t*>(frameBytes + 0x3C);
    auto& savedResult = CSF_AT(uint32_t*, frameBytes, 0x40);
    savedResult = nullptr;

    NetSession_ConstructSpawnRecordIterator(iterator, nullptr);
    if (CSF_AT(int, iterator, 0) != 0)
    {
        NetSession_ResetRecordIterator(iterator, nullptr);
        NetSession_AdvanceRecordIterator(iterator, nullptr, 1);
    }
    else
    {
        *emptyReference = 0;
        NetSession_AssignRecordReference(
            currentReference, nullptr, emptyReference);
    }

    int hasCurrent = CSF_AT(int, currentReference, 0x4);
    while (hasCurrent != 0)
    {
        if (result != nullptr)
            break;

        void* current;
        if (hasCurrent != 0)
        {
            current = CSF_AT(void*, currentReference, 0);
        }
        else
        {
            Game_RaiseStatusNotice(0, 0, 0xA0000007, 0);
            current = nullptr;
        }

        if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, current, 0)[0x34 / 4])(current, nullptr))
        {
            int currentValue;
            if (CSF_AT(int, currentReference, 0x4) != 0)
            {
                currentValue = CSF_AT(int, currentReference, 0);
            }
            else
            {
                Game_RaiseStatusNotice(0, 0, 0xA0000007, 0);
                currentValue = 0;
            }

            NetSession_ConstructRecordReference(
                temporaryReference, nullptr, currentValue);
            NetSession_CopyRecordReference(
                convertedReference, nullptr,
                reinterpret_cast<int*>(temporaryReference));
            NetSession_ReleaseRecordReference(temporaryReference, nullptr);

            auto* candidate = static_cast<uint32_t*>(
                NetSession_GetSpawnRecordFromReference(convertedReference));

            // The ID comparison intentionally precedes the null check.
            if ((excludedId == -1 ||
                 CSF_AT(int, candidate, 0xC) != excludedId) &&
                candidate != nullptr &&
                CSF_AT(int, candidate, 0xC) != -1 &&
                CSF_AT(int, candidate, 0x8) == 0 &&
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, candidate, 0)[0x4 / 4])(
                        candidate, nullptr) == 0)
            {
                savedResult = candidate;
            }

            NetSession_ReleaseRecordReference(convertedReference, nullptr);
            result = savedResult;
        }

        NetSession_AdvanceRecordIterator(iterator, nullptr, 0);
        hasCurrent = CSF_AT(int, currentReference, 0x4);
    }

    NetSession_FinishRecordIterator(iterator, nullptr, 0);
    excludedId = 0;
    NetSession_AssignRecordReference(
        currentReference, nullptr,
        reinterpret_cast<uint32_t*>(&excludedId));
    NetSession_ReleaseRecordReference(currentReference, nullptr);
    return result;
}

HOOK(0x0073DCE0, NetSession_GetNextPendingSpawnRecord, "auto");
