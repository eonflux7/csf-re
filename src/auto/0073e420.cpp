#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, NetSession_GetHandleRegistry, 0x0064AD00, (int a1));
CSF_FUNC(void, __fastcall, NetSession_ResetParticipantCursor, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_AdvanceParticipantCursor, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_ConstructParticipantReference, 0x00651A60, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_ReleaseParticipantReference, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_AssignParticipantHandle, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, NetSession_CopyParticipantReference, 0x00651D30, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, NetSession_CloseParticipantCursor, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, NetSession_OpenParticipantCursor, 0x0073D7B0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, NetSession_ResolveParticipant, 0x0074A9C0, (void* a1));

static int __stdcall NetSession_FindParticipant(int (__stdcall *participantHandle)(int))
{
    // Preserve the cursor's adjacency to its embedded reference and iterator state.
    alignas(8) uint8_t frameBytes[0x4C];
    void* cursor = frameBytes;
    void* currentReference = frameBytes + 0x04;
    void* copiedReference = frameBytes + 0x24;
    void* temporaryReference = frameBytes + 0x30;
    void* participantReference = frameBytes + 0x3C;
    uint32_t* emptyHandle = reinterpret_cast<uint32_t*>(frameBytes + 0x48);

    NetSession_OpenParticipantCursor(cursor, nullptr);
    int result = 0;

    if (CSF_AT(int, cursor, 0))
    {
        NetSession_ResetParticipantCursor(cursor, nullptr);
        NetSession_AdvanceParticipantCursor(cursor, nullptr, 1);
    }
    else
    {
        *emptyHandle = 0;
        NetSession_AssignParticipantHandle(currentReference, nullptr, emptyHandle);
    }

    int hasCurrent = CSF_AT(int, currentReference, 0x04);
    while (hasCurrent)
    {
        if (result)
            break;

        int current;
        if (hasCurrent)
        {
            current = CSF_AT(int, currentReference, 0);
        }
        else
        {
            Game_RaiseStatusNotice(0, 0, -1610612729, 0);
            current = 0;
        }

        NetSession_ConstructParticipantReference(temporaryReference, nullptr, current);
        NetSession_CopyParticipantReference(
            copiedReference, nullptr, reinterpret_cast<int*>(temporaryReference));
        NetSession_ReleaseParticipantReference(temporaryReference, nullptr);
        NetSession_CopyParticipantReference(
            participantReference, nullptr, reinterpret_cast<int*>(copiedReference));

        void* entry = CSF_AT(void*, participantReference, 0);
        if (entry)
        {
            void* registry = reinterpret_cast<void*>(
                NetSession_GetHandleRegistry(
                    static_cast<int>(CSF_AT(uint32_t, entry, 0x2C) >> 22)));

            if (((bool (__fastcall*)(void*, void*, int))
                     CSF_AT(void**, registry, 0)[0x3C / 4])(registry, nullptr, 1))
            {
                entry = CSF_AT(void*, participantReference, 0);
            }
            else
            {
                Game_RaiseStatusNotice(0, 0, -536674292, 0);
                entry = nullptr;
            }
        }
        else
        {
            Game_RaiseStatusNotice(0, 0, -1610416124, 0);
            entry = nullptr;
        }

        uint32_t handle = CSF_AT(uint32_t, entry, 0x2C);
        if ((handle & 0x3FFFFF) == 0)
        {
            Game_RaiseStatusNotice(0, 0, -536870898, 0);
            handle = 0;
        }

        if (handle == reinterpret_cast<uint32_t>(participantHandle))
        {
            result = reinterpret_cast<int>(
                NetSession_ResolveParticipant(participantReference));
        }

        NetSession_AdvanceParticipantCursor(cursor, nullptr, 0);
        NetSession_ReleaseParticipantReference(participantReference, nullptr);
        NetSession_ReleaseParticipantReference(copiedReference, nullptr);
        hasCurrent = CSF_AT(int, currentReference, 0x04);
    }

    NetSession_CloseParticipantCursor(cursor, nullptr, 0);
    participantHandle = nullptr;
    NetSession_AssignParticipantHandle(
        currentReference, nullptr, reinterpret_cast<uint32_t*>(&participantHandle));
    NetSession_ReleaseParticipantReference(currentReference, nullptr);
    return result;
}

HOOK(0x0073E420, NetSession_FindParticipant, "auto");
