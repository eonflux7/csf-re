#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, NetSession_GetHandleRegistry, 0x0064AD00, (int a1));
CSF_FUNC(void, __fastcall, NetSession_PreparePlayerIterator, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_AdvancePlayerIterator, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_ConstructPlayerReference, 0x00651A60, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_ReleasePlayerReference, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_AssignPlayerReference, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, NetSession_CopyPlayerReference, 0x00651D30, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, NetSession_ResetPlayerIterator, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, NetSession_GetPlayerIterator, 0x0073D710, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, NetSession_ResolvePlayerReference, 0x0074A9C0, (void* a1));

static int __stdcall NetSession_FindPlayerById(uint32_t playerId)
{
    // Keep the iterator and its embedded reference contiguous, as in the
    // original frame. The remaining regions hold temporary references.
    alignas(8) uint8_t frameBytes[0x40];
    void* iterator = frameBytes;
    void* currentReference = frameBytes + 0x4;
    void* temporaryReference = frameBytes + 0x24;
    void* playerReference = frameBytes + 0x30;
    uint32_t* emptyReference = reinterpret_cast<uint32_t*>(frameBytes + 0x3C);

    NetSession_GetPlayerIterator(iterator, nullptr);
    int result = 0;

    if (CSF_AT(int, iterator, 0))
    {
        NetSession_PreparePlayerIterator(iterator, nullptr);
        NetSession_AdvancePlayerIterator(iterator, nullptr, 1);
    }
    else
    {
        *emptyReference = 0;
        NetSession_AssignPlayerReference(currentReference, nullptr, emptyReference);
    }

    int available = CSF_AT(int, currentReference, 0x4);
    while (available)
    {
        if (result)
            break;

        int currentHandle;
        if (available)
        {
            currentHandle = CSF_AT(int, currentReference, 0);
        }
        else
        {
            Game_RaiseStatusNotice(0, 0, -1610612729, 0);
            currentHandle = 0;
        }

        NetSession_ConstructPlayerReference(temporaryReference, nullptr, currentHandle);
        NetSession_CopyPlayerReference(
            playerReference, nullptr, reinterpret_cast<int*>(temporaryReference));
        NetSession_ReleasePlayerReference(temporaryReference, nullptr);

        void* handleObject = CSF_AT(void*, playerReference, 0);
        if (handleObject)
        {
            void* registry = reinterpret_cast<void*>(
                NetSession_GetHandleRegistry(
                    static_cast<int>(CSF_AT(uint32_t, handleObject, 0x2C) >> 22)));

            bool valid = ((bool(__fastcall*)(void*, void*, int))
                CSF_AT(void**, registry, 0)[0x3C / 4])(registry, nullptr, 1);

            if (valid)
            {
                handleObject = CSF_AT(void*, playerReference, 0);
            }
            else
            {
                Game_RaiseStatusNotice(0, 0, -536674292, 0);
                handleObject = nullptr;
            }
        }
        else
        {
            Game_RaiseStatusNotice(0, 0, -1610416124, 0);
            handleObject = nullptr;
        }

        // The original still reads this field after a failed validation.
        uint32_t candidateId = CSF_AT(uint32_t, handleObject, 0x2C);
        if ((candidateId & 0x3FFFFF) == 0)
        {
            Game_RaiseStatusNotice(0, 0, -536870898, 0);
            candidateId = 0;
        }

        if (candidateId == playerId)
        {
            result = static_cast<int>(reinterpret_cast<uintptr_t>(
                NetSession_ResolvePlayerReference(playerReference)));
        }

        NetSession_AdvancePlayerIterator(iterator, nullptr, 0);
        NetSession_ReleasePlayerReference(playerReference, nullptr);
        available = CSF_AT(int, currentReference, 0x4);
    }

    NetSession_ResetPlayerIterator(iterator, nullptr, 0);
    playerId = 0;
    NetSession_AssignPlayerReference(currentReference, nullptr, &playerId);
    NetSession_ReleasePlayerReference(currentReference, nullptr);
    return result;
}

HOOK(0x0073E2C0, NetSession_FindPlayerById, "auto");
