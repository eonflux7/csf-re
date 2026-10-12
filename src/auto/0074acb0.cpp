#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void, __fastcall, NetSession_ResetSelection, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_SetSelectionMode, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_InitializeObjectReference, 0x00651A60, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SessionScope_Begin, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, NetSession_CopyObjectReference, 0x00651D30, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, NetDdlObjectManager_GetReferenceValue156, 0x00652DD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetDdlObjectManager_GetReferenceValue160, 0x00652E50, (void* self, void* edx));
CSF_FUNC(unsigned int*, __fastcall, SessionFlagBuffer_Init, 0x006530D0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SessionFlagBuffer_SetMode, 0x0065E130, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_NetDdlObjectType, 0x008B9A3C);

static void __stdcall NetDdlObjectManager_ResolveObjectReference(int objectAddress)
{
    auto* object = reinterpret_cast<uint8_t*>(objectAddress);
    if (!object || CSF_AT(int, object, 0xC) != 0)
        return;

    // Keep the original frame's adjacent objects together: initialization
    // of the flag buffer also establishes the selection iterator.
    alignas(8) uint8_t frame[0x48];
    void* flagBuffer = frame;
    void* iterator = frame + 0x4;
    void* temporaryReference = frame + 0x24;
    void* reference = frame + 0x30;

    CSF_AT(uint32_t, frame, 0x44) = 0;
    CSF_AT(uint32_t, frame, 0x3C) = CSF_AT(uint32_t, object, -0x6A4);
    bool found = false;

    SessionFlagBuffer_Init(flagBuffer, nullptr);
    if (CSF_AT(int, frame, 0) != 0)
    {
        NetSession_ResetSelection(flagBuffer, nullptr);
        NetSession_SetSelectionMode(flagBuffer, nullptr, 1);
    }
    else
    {
        CSF_AT(uint32_t, frame, 0x40) = 0;
        SessionScope_Begin(iterator, nullptr,
                           reinterpret_cast<uint32_t*>(frame + 0x40));
    }

    int hasSelection = CSF_AT(int, frame, 0x8);
    while (hasSelection != 0 && !found)
    {
        int selection;
        if (hasSelection != 0)
        {
            selection = CSF_AT(int, frame, 0x4);
        }
        else
        {
            Game_RaiseStatusNotice(0, 0, -1610612729, 0);
            selection = 0;
        }

        NetSession_InitializeObjectReference(temporaryReference, nullptr, selection);
        NetSession_CopyObjectReference(reference, nullptr,
                                       reinterpret_cast<int*>(temporaryReference));
        SpawnPoints_ReleaseEntry(temporaryReference, nullptr);

        void* candidate = nullptr;
        bool matches = false;
        void* referencedObject = CSF_AT(void*, reference, 0);
        if (!referencedObject)
        {
            Game_RaiseStatusNotice(0, 0, -1610416124, 0);
        }
        else
        {
            int index = static_cast<int>(
                CSF_AT(uint32_t, referencedObject, 0x2C) >> 22);
            void* indexedObject = reinterpret_cast<void*>(LookupObjectByIndex(index));
            bool valid =
                (reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, indexedObject, 0)[0x3C / 4]))(
                        indexedObject, nullptr, g_NetDdlObjectType);

            if (!valid)
            {
                Game_RaiseStatusNotice(0, 0, -536674292, 0);
            }
            else
            {
                candidate = CSF_AT(void*, reference, 0);
                if (candidate)
                {
                    uint32_t identifier = CSF_AT(uint32_t, candidate, 0x2C);
                    CSF_AT(uint32_t, frame, 0x44) |= 1u;
                    if ((identifier & 0x3FFFFF) == 0)
                    {
                        Game_RaiseStatusNotice(0, 0, -536870898, 0);
                        identifier = 0;
                    }
                    matches = identifier == CSF_AT(uint32_t, frame, 0x3C);
                }
            }
        }

        if ((CSF_AT(uint32_t, frame, 0x44) & 1u) != 0)
            CSF_AT(uint32_t, frame, 0x44) &= ~1u;

        if (matches)
        {
            CSF_AT(int, object, 0x9C) =
                NetDdlObjectManager_GetReferenceValue156(candidate, nullptr);
            CSF_AT(int, object, 0xA0) =
                NetDdlObjectManager_GetReferenceValue160(candidate, nullptr);
            found = true;
        }

        NetSession_SetSelectionMode(flagBuffer, nullptr, 0);
        SpawnPoints_ReleaseEntry(reference, nullptr);
        hasSelection = CSF_AT(int, frame, 0x8);
    }

    SessionFlagBuffer_SetMode(flagBuffer, nullptr, 0);
    uint32_t emptyScope = 0;
    SessionScope_Begin(iterator, nullptr, &emptyScope);
    SpawnPoints_ReleaseEntry(iterator, nullptr);
}

HOOK(0x0074ACB0, NetDdlObjectManager_ResolveObjectReference, "auto");
