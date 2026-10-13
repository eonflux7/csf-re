#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Bound original callees (signatures per packet).
CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(char*, __cdecl, NodeAllocator_AllocBlock, 0x00647070, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (int a1, int a2, int a3));
CSF_FUNC(char, __fastcall, NetChat_CopyTextBytes, 0x00655CE0, (void* self, void* edx, char* a2, unsigned int a3));
CSF_FUNC(int, __fastcall, ReadCounterWord, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(std::uint32_t*, __fastcall, S_004f21d0_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));

static void __cdecl NetChatDDL_ReadMessageText(C_008622e8* a1, S_004f21d0* a2)
{
    // Stack slot [ebp-4] (4 bytes). Not zeroed: the original only reads the
    // low word that ReadCounterWord leaves behind (movzx word ptr).
    alignas(4) uint8_t counterSlot[4];
    ReadCounterWord(a1, nullptr, reinterpret_cast<char*>(counterSlot));

    uint16_t lengthWord;
    std::memcpy(&lengthWord, counterSlot, sizeof(lengthWord));

    int charCount = static_cast<int>(lengthWord) + 1;

    uint32_t* allocator = NetSession_GetNodeAllocator();
    char* block = NodeAllocator_AllocBlock(
        static_cast<int>(reinterpret_cast<uintptr_t>(allocator)),
        2 * charCount + 4,
        static_cast<int>(reinterpret_cast<uintptr_t>(CSF_STR(0x868954, "NZMainTypes.h"))),
        29,
        9);

    CSF_AT(int, block, 0) = charCount;

    char* textBytes = block + 4;
    auto* text = reinterpret_cast<int16_t*>(textBytes);

    std::memcpy(&lengthWord, counterSlot, sizeof(lengthWord));
    NetChat_CopyTextBytes(a1, nullptr, textBytes, lengthWord);

    text[lengthWord] = 0;

    S_004f21d0_AssignText(reinterpret_cast<std::uint32_t*>(a2), nullptr, text);

    uint32_t* freeAllocator = NetSession_GetNodeAllocator();
    NetSession_FreeListNode(
        static_cast<int>(reinterpret_cast<uintptr_t>(freeAllocator)),
        static_cast<int>(reinterpret_cast<uintptr_t>(textBytes - 4)),
        9);
}

HOOK(0x00750EB0, NetChatDDL_ReadMessageText, "auto");
