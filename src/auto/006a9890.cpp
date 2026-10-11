#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, ReturnZero, 0x004B2A30, ());
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ResolveUpdateContext, 0x006538E0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char, __fastcall, Sub_655520, 0x00655520, (void* self, void* edx, int a, int b));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __stdcall, GlobalObject_ApplyMode, 0x0073C7F0, (char mode));

using SleepFunction = void (__stdcall*)(uint32_t);
CSF_GLOBAL(SleepFunction, g_SleepImportSlot, 0x0082D05C);
CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectLookup, 0x008B9A90);
CSF_GLOBAL(void*, g_CharacterSelectEntries, 0x008B9AB4);
CSF_GLOBAL(uint8_t, g_GatheringCancelled, 0x008BB41C);
CSF_GLOBAL(uint8_t, g_GatheringWaitFinished, 0x008BB41D);

static void __cdecl NetGatheringDdl_WaitForGathering(void* gathering)
{
    if (!g_GatheringCancelled)
    {
        GetGlobalDword_73C020();
        GlobalObject_ApplyMode(0);
    }

    if (((int (__fastcall*)(void*, void*))
         CSF_AT(void**, gathering, 0)[0x1C / 4])(gathering, nullptr) == 1)
    {
        const SleepFunction sleep = g_SleepImportSlot;

        while (!g_GatheringCancelled)
        {
            const uint32_t index = static_cast<uint32_t>(ReturnZero());
            void* lookup = nullptr;
            bool validEntry = false;

            if (!index)
            {
                lookup = &g_MenuMpCharacterSelectLookup;
                validEntry = true;
            }
            else
            {
                void* entries = g_CharacterSelectEntries;
                const uint32_t begin = CSF_AT(uint32_t, entries, 0x4);
                if (begin)
                {
                    const uint32_t end = CSF_AT(uint32_t, entries, 0x8);
                    const uint32_t count = static_cast<uint32_t>(
                        static_cast<int32_t>(end - begin) >> 2);

                    if (index < count)
                    {
                        lookup = reinterpret_cast<void**>(begin)[index];
                        validEntry = true;
                    }
                }
            }

            int context;
            if (validEntry)
            {
                context = NetGatheringDdl_ResolveUpdateContext(
                    lookup, nullptr, 4);
            }
            else
            {
                Game_RaiseStatusNotice(0, 0, 0xE0000003, 0);
                context = -1;
            }

            void* updateTarget = nullptr;
            if (context)
            {
                void* inner = CSF_AT(
                    void*, reinterpret_cast<void*>(static_cast<uint32_t>(context)),
                    0xC);
                if (inner)
                    updateTarget = CSF_AT(void*, inner, 0x8);
            }

            Sub_655520(updateTarget, nullptr, -1, 1);
            sleep(200);

            if (((int (__fastcall*)(void*, void*))
                 CSF_AT(void**, gathering, 0)[0x1C / 4])(gathering, nullptr) != 1)
                break;
        }
    }

    if (!g_GatheringCancelled)
    {
        GetGlobalDword_73C020();
        GlobalObject_ApplyMode(1);
    }

    g_GatheringWaitFinished = 1;
}

HOOK(0x006A9890, NetGatheringDdl_WaitForGathering, "auto");
