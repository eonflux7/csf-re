#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, NetSession_PrepareSessionState, 0x0064B880,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_SetSessionState, 0x0064B900,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_InitializeReferenceFromValue, 0x00651A60,
         (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_DestroyReference, 0x00651B40,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_AssignReferenceFromPointer, 0x00651C70,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, NetSession_CopyReference, 0x00651D30,
         (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, NetSession_ResetSessionState, 0x0065E130,
         (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, ResolveSpawnPointName, 0x006B29A0,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetSession_HasReferenceValue, 0x0073C980,
         (void* self, void* edx));
CSF_FUNC(int*, __fastcall, NetSession_ConstructSessionState, 0x0073D910,
         (void* self, void* edx));

static char* __cdecl NetSession_GetSpawnPointNameSuffix()
{
    // Preserve the contiguous storage used by the original stack objects.
    alignas(8) uint8_t frameBytes[0x40];
    void* sessionState = frameBytes;
    void* sessionReference = frameBytes + 0x4;
    void* temporaryReference = frameBytes + 0x24;
    void* resolvedReference = frameBytes + 0x30;
    uint32_t* emptyValue = reinterpret_cast<uint32_t*>(frameBytes + 0x3C);

    NetSession_ConstructSessionState(sessionState, nullptr);
    char* result = nullptr;

    if (CSF_AT(int, sessionState, 0) != 0)
    {
        NetSession_PrepareSessionState(sessionState, nullptr);
        NetSession_SetSessionState(sessionState, nullptr, 1);
    }
    else
    {
        *emptyValue = 0;
        NetSession_AssignReferenceFromPointer(sessionReference, nullptr, emptyValue);
    }

    if (CSF_AT(int, sessionReference, 0x4) != 0)
    {
        NetSession_InitializeReferenceFromValue(
            temporaryReference, nullptr, CSF_AT(int, sessionReference, 0));
        NetSession_CopyReference(
            resolvedReference, nullptr, reinterpret_cast<int*>(temporaryReference));
        NetSession_DestroyReference(temporaryReference, nullptr);

        const char hasValue = NetSession_HasReferenceValue(resolvedReference, nullptr);
        void* spawnPoint = reinterpret_cast<void*>(
            CSF_AT(uint32_t, resolvedReference, 0));
        result = ResolveSpawnPointName(hasValue != 0 ? spawnPoint : nullptr, nullptr) + 7;

        NetSession_DestroyReference(resolvedReference, nullptr);
    }

    NetSession_ResetSessionState(sessionState, nullptr, 0);
    *emptyValue = 0;
    NetSession_AssignReferenceFromPointer(sessionReference, nullptr, emptyValue);
    NetSession_DestroyReference(sessionReference, nullptr);
    return result;
}

HOOK(0x0073DF80, NetSession_GetSpawnPointNameSuffix, "auto");
