#include <cstdint>

#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// 0x8B0694: pointer to the multiplayer character-select state (0 when the menu is not active).
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x8B0694);

// 0x550CC0: __cdecl, no args; returns the global menu object as an integer (0 when absent).
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());

// 0x748380: __thiscall helper on the character sub-object (pops 1 stack arg).
CSF_FUNC(bool, __fastcall, Character_748380, 0x748380, (void* self, void* edx, char a2));

// 0x7483B0: __thiscall helper on the character sub-object (pops 0 stack args).
CSF_FUNC(int, __fastcall, Character_7483B0, 0x7483B0, (void* self, void* edx));

// 0x542820: when the character-select menu is present and its current character object passes its
// virtual "valid/ready" check (vtable slot 2), run the two confirmation helpers on the embedded
// sub-object located at (character - 0x308). The original tail-jumps (0x542830) into the block at
// 0x551C30, whose body is reproduced inline below exactly as Hex-Rays merged it.
static int __cdecl MenuMpCharacterSelect_CommitCharacterSelection()
{
    int result = GetGlobal8B0688();
    if (!result)
        return result;

    // Second fetch: side effecting; its value only ever lands in the (unused) ecx of the tail target.
    GetGlobal8B0688();

    result = static_cast<int>(g_CharacterSelectState8B0694);
    if (!result)
        return result;

    result = static_cast<int>(*reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(result) + 4u));
    if (!result)
        return result;

    const int characterSubObj = result - 776;   // embedded object at -0x308
    if (characterSubObj == 0)
        return result;

    result -= 776;
    if (result == 0)
        return result;

    // Virtual call (0x551C5C): this = the character object (result + 776), vtable slot 2.
    void* characterObj = reinterpret_cast<void*>(static_cast<uintptr_t>(result) + 776u);
    void** vtable = *reinterpret_cast<void***>(characterObj);
    result = reinterpret_cast<int(__fastcall*)(void*, void*)>(vtable[2])(characterObj, nullptr);
    if (!(result & 0xFF))
        return result;

    void* subObj = reinterpret_cast<void*>(static_cast<uintptr_t>(characterSubObj));
    Character_7483B0(subObj, nullptr);
    return Character_748380(subObj, nullptr, 1);
}

HOOK(0x542820, MenuMpCharacterSelect_CommitCharacterSelection, "auto");
