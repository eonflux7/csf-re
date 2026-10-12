#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, FormatCountTextCore_6966E0, 0x006966E0, (void* a1, char a2, void** a3, int a4, int a5, int a6, int a7, unsigned int a8, int* a9));

static void* __cdecl FormatCountTextBuffer(void* a1, char a2, void* Block, int a4, int a5, int a6, int a7, std::uint32_t a8, char arg20)
{
    // 28-byte string object on the stack; its dwords are passed as the
    // trailing arguments of FormatCountTextCore_6966E0.
    alignas(8) std::uint8_t stringBytes[0x1C];
    int a9 = static_cast<int>(reinterpret_cast<std::uintptr_t>(&arg20));

    CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;
    CSF_AT(int, stringBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, stringBytes, 4) = 0;

    AssignStringRange_488830(stringBytes, nullptr, &a2, 0, 0xFFFFFFFFu);

    void* resultText = FormatCountTextCore_6966E0(
        a1,
        CSF_AT(char, stringBytes, 0),
        CSF_AT(void**, stringBytes, 4),
        CSF_AT(int, stringBytes, 8),
        CSF_AT(int, stringBytes, 0xC),
        CSF_AT(int, stringBytes, 0x10),
        CSF_AT(int, stringBytes, 0x14),
        CSF_AT(std::uint32_t, stringBytes, 0x18),
        &a9);

    if (a8 >= 8u && Block) {
        Mem_FreeDebug(Block);
    }
    return resultText;
}

HOOK(0x00696A80, FormatCountTextBuffer, "auto");
