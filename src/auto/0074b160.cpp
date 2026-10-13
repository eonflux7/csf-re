#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// Bind-as lines from the packet. InitializeTemporaryString returns a pointer in eax, and
// the original passes that value on, so it is declared void* here.
CSF_FUNC(void*, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));

// Returns true when the member name at a2+8 matches "m_uValor"; if so, writes
// the object's first dword as a decimal string into a3 and returns 1.
static char __fastcall NetGame_FormatEndTimeMember(std::int32_t* self, void* /*edx*/, void* a2, void* a3)
{
    // Stack temporary the original passes as `this` to the string helpers ([ebp-4], 4 bytes).
    alignas(8) std::uint8_t varTempStringBytes[4];

    void* nameHandle = InitializeTemporaryString(varTempStringBytes, nullptr,
                                                 static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(CSF_STR(0x8680E8, "m_uValor"))));
    char nameMatches = NameMatchesString(static_cast<std::uint8_t*>(a2) + 8, nullptr, reinterpret_cast<void**>(nameHandle));
    DestroyTemporaryString(varTempStringBytes, nullptr);

    if (!nameMatches)
        return 0;

    FormatValueString(reinterpret_cast<void**>(a3),
                      reinterpret_cast<char*>(CSF_ADDR_DATA(0x83090C)),
                      self[0],
                      self[1]);
    return 1;
}

HOOK(0x0074B160, NetGame_FormatEndTimeMember, "auto");
