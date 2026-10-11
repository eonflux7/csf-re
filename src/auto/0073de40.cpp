#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __fastcall, NetSession_TestSelectedObjectState, 0x0064AF40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_AdvanceIterator, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_SetIteratorTraversalMode, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetSession_ProcessSelectedObject, 0x0064D1A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_ReleaseIteratorLink, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_ResetIteratorLink, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, NetSession_AssignIteratorSentinel, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, NetSession_InitializeSelectionIterator, 0x0073D710, (void* self, void* edx));

static void __cdecl NetSession_ProcessSelection()
{
    // The iterator contains a context followed by an embedded 0x20-byte link.
    alignas(8) std::uint8_t iteratorBytes[0x24];
    alignas(8) std::uint8_t resetValueBytes[0x4];
    auto& resetValue = *reinterpret_cast<std::uint32_t*>(resetValueBytes);
    void* iteratorLink = iteratorBytes + 4;

    NetSession_InitializeSelectionIterator(iteratorBytes, nullptr);
    if (CSF_AT(std::uint32_t, iteratorBytes, 0))
    {
        NetSession_AdvanceIterator(iteratorBytes, nullptr);
        NetSession_SetIteratorTraversalMode(iteratorBytes, nullptr, 1);
    }
    else
    {
        resetValue = 0;
        NetSession_ResetIteratorLink(iteratorLink, nullptr, &resetValue);
    }

    while (CSF_AT(std::uint32_t, iteratorBytes, 8))
    {
        void* object = CSF_AT(void*, iteratorBytes, 4);
        if (!((bool(__fastcall*)(void*, void*))
                  CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr))
        {
            if (CSF_AT(std::uint32_t, iteratorBytes, 8))
                object = CSF_AT(void*, iteratorBytes, 4);
            else
            {
                Game_RaiseStatusNotice(0, 0, 0xA0000007, 0);
                object = nullptr;
            }

            if (static_cast<std::uint8_t>(Sub_64B250(object, nullptr)))
            {
                if (CSF_AT(std::uint32_t, iteratorBytes, 8))
                    object = CSF_AT(void*, iteratorBytes, 4);
                else
                {
                    Game_RaiseStatusNotice(0, 0, 0xA0000007, 0);
                    object = nullptr;
                }

                if (!((bool(__fastcall*)(void*, void*, int))
                          CSF_AT(void**, object, 0)[0xC / 4])(object, nullptr, 0))
                {
                    if (CSF_AT(std::uint32_t, iteratorBytes, 8))
                        object = CSF_AT(void*, iteratorBytes, 4);
                    else
                    {
                        Game_RaiseStatusNotice(0, 0, 0xA0000007, 0);
                        object = nullptr;
                    }

                    if (!static_cast<std::uint8_t>(
                            NetSession_TestSelectedObjectState(object, nullptr)))
                    {
                        if (CSF_AT(std::uint32_t, iteratorBytes, 8))
                            object = CSF_AT(void*, iteratorBytes, 4);
                        else
                        {
                            Game_RaiseStatusNotice(0, 0, 0xA0000007, 0);
                            object = nullptr;
                        }

                        NetSession_ProcessSelectedObject(object, nullptr);
                    }
                }
            }
        }

        NetSession_SetIteratorTraversalMode(iteratorBytes, nullptr, 0);
    }

    NetSession_AssignIteratorSentinel(iteratorBytes, nullptr, 0);
    resetValue = 0;
    NetSession_ResetIteratorLink(iteratorLink, nullptr, &resetValue);
    NetSession_ReleaseIteratorLink(iteratorLink, nullptr);
}

HOOK(0x0073DE40, NetSession_ProcessSelection, "auto");
