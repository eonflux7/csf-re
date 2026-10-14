#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(int, __cdecl, MenuMessageBox_TestEntryAgainstFlag, 0x00695500, (uint16_t* entry, char* flagByte));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static S_004f21d0* __cdecl InitStringFromLiteral(S_004f21d0* a1, int a2, int a3)
{
    bool wasConstructed = false;

    // Stack string object built by ConstructWideString (ebp-0x20 .. ebp-0x01).
    alignas(8) uint8_t stringBytes[0x20];
    // 00692730 xor ebx,ebx; 00692733 mov [ebp-4],ebx.
    CSF_AT(std::uint32_t, stringBytes, 0x1C) = 0;

    int descriptionId = MenuExtras_ParseDescriptionId(reinterpret_cast<const char*>(a2));
    int entry = MenuMessageBox_ResolveEntryByKey(descriptionId, a3);
    wchar_t* resolvedEntry = reinterpret_cast<wchar_t*>(entry);

    void* sourceString = a1;
    if (entry != 0 &&
        MenuMessageBox_TestEntryAgainstFlag(reinterpret_cast<uint16_t*>(resolvedEntry),
                                            reinterpret_cast<char*>(&g_byte_82E65C)) != 0)
    {
        wasConstructed = true;
        sourceString = ConstructWideString(stringBytes, nullptr, resolvedEntry);
    }

    AssignStringRange_488830(a1, nullptr, sourceString, 0, 0xFFFFFFFFu);

    if (wasConstructed &&
        CSF_AT(std::uint32_t, stringBytes, 0x18) >= 8 &&
        CSF_AT(void*, stringBytes, 4) != nullptr)
    {
        Mem_FreeDebug(CSF_AT(void*, stringBytes, 4));
    }

    return a1;
}

HOOK(0x00692720, InitStringFromLiteral, "auto");
