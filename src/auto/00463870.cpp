#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(bool, __stdcall, IsCompatibleTargetPair, 0x0045BB30, (int a1, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Follows the two link records in a4 to their handle blocks, resolves both handles on the
// current scene, and when the source entity has a message dispatcher and the pair is
// compatible, sends the dispatcher a 5-dword message (type 27) carrying the target's two
// words. Afterwards it latches the source's two words into self+0x60/+0x64 and clears the
// flag byte at self+0x82. Returns the low byte of the working result (char return).
static char __fastcall DispatchLinkedMessageAndLatch(void* self, void* /*edx*/, int /*a2*/, int a3, uint32_t* a4)
{
    // Read order mirrors the original: a4[1] first, then a4[0] and the source chain,
    // then the target handle block.
    const uint32_t targetRecord = a4[1];
    const uint32_t sourceRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(a4[0]), 4);
    const uint32_t sourceHandle = sourceRecord + 4;
    int resultWord = CSF_AT(int, reinterpret_cast<void*>(sourceRecord), 12);
    const uint32_t targetHandle = CSF_AT(uint32_t, reinterpret_cast<void*>(targetRecord), 4) + 4;

    if (resultWord >= 0)
    {
        resultWord = CSF_AT(int, reinterpret_cast<void*>(targetHandle), 8);
        if (resultWord >= 0)
        {
            const int sourceEntity = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(sourceHandle));
            resultWord = sourceEntity;
            if (sourceEntity != 0)
            {
                if (CSF_AT(int, reinterpret_cast<void*>(sourceEntity), 52) != 0)
                {
                    const int targetEntity = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(targetHandle));
                    const uint8_t compatByte = IsCompatibleTargetPair(a3, targetEntity) ? 1 : 0;
                    resultWord = (resultWord & ~0xFF) | compatByte;

                    if (compatByte != 0)
                    {
                        // Target words are read after the compatibility check, as in the original.
                        const uint32_t targetWordA = CSF_AT(uint32_t, reinterpret_cast<void*>(targetHandle), 4);
                        const uint32_t targetWordB = CSF_AT(uint32_t, reinterpret_cast<void*>(targetHandle), 8);

                        uint32_t message[5];
                        message[0] = 27;
                        message[1] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
                        message[2] = targetWordA;
                        message[3] = targetWordB;
                        message[4] = 0xFFFFFFFFu;

                        // The dispatcher is re-read from the saved source entity after the compatibility call.
                        void* messageDispatcher = reinterpret_cast<void*>(
                            CSF_AT(uint32_t, reinterpret_cast<void*>(sourceEntity), 52));
                        const int dispatchResult =
                            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*)>(
                                CSF_AT(void**, messageDispatcher, 0)[42])(messageDispatcher, nullptr, message);
                        resultWord = (resultWord & ~0xFF) | (dispatchResult & 0xFF);

                        // Source words are re-read from memory after the dispatch call.
                        CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, reinterpret_cast<void*>(sourceHandle), 4);
                        CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, reinterpret_cast<void*>(sourceHandle), 8);
                        CSF_AT(uint8_t, self, 0x82) = 0;
                    }
                }
            }
        }
    }
    return static_cast<char>(resultWord);
}

HOOK(0x00463870, DispatchLinkedMessageAndLatch, "auto");
