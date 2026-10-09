#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, IsSceneEntryTypeFlagged, 0x00755880, (int a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall DispatchHandleEntryCommand(int /*unusedA1*/, int /*unusedA2*/, uint32_t* commandSource)
{
    // commandSource -> *commandSource is the owner object; owner+4 is the source node.
    uint8_t* commandOwner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(commandSource[0]));
    uint8_t* sourceNode = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, commandOwner, 4)));
    int handleIndex = CSF_AT(int, sourceNode, 12);

    // Default result when no entry is resolved: address of the handle field inside the source node.
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(sourceNode + 4));

    if (handleIndex >= 0)
    {
        uint32_t entryValue = static_cast<uint32_t>(CHandleTable_Resolve(g_pCurrentScene, nullptr, result));
        result = static_cast<int>(entryValue);
        if (entryValue != 0)
        {
            uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryValue));

            result = IsSceneEntryTypeFlagged(CSF_AT(int, entry, 0x50));
            if (static_cast<uint8_t>(result) != 0)
            {
                int entryType = CSF_AT(int, entry, 0x50);
                result = entryType;
                if (entryType >= 4 && (entryType <= 7 || entryType == 19))
                {
                    uint8_t* argNode = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, commandSource, 4)));
                    uint8_t* argRecord = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, argNode, 4)));
                    uint8_t commandArg = CSF_AT(uint8_t, argRecord, 1);

                    void* entryHandler = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entry, 0x34)));
                    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, entryHandler, 0)[50])(
                        entryHandler, nullptr, static_cast<int>(commandArg));
                }
            }
        }
    }
    return result;
}

HOOK(0x460C30, DispatchHandleEntryCommand, "auto");
