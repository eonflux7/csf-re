#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Looks up a property by keyword under the given node.
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

// Loads a binding table from a CONTROLES node into the given address (cdecl).
CSF_FUNC(int, __cdecl, Cheats_LoadControlBindings_428980, 0x00428980, (S_00692b80* node, int bindingsAddress, int));

CSF_GLOBAL(const char*, g_pszKey_CONTROLS, 0x00899A94);
CSF_GLOBAL(const char*, g_pszKey_CONTROLES, 0x00899A44);

// Finds CONTROLS -> CONTROLES in the options node and loads the control bindings
// into the object's embedded binding table at offset 4. Returns false if either key is missing.
static bool __fastcall S_0050a6c0_LoadControlBindings(S_0050a6c0* self, void* /*edx*/, S_00692b80* optionsNode)
{
    std::uint8_t* controlsNode = FindProperty(
        reinterpret_cast<std::uint8_t*>(optionsNode), nullptr,
        const_cast<char*>(g_pszKey_CONTROLS));
    if (controlsNode == nullptr)
        return false;

    std::uint8_t* controlesNode = FindProperty(
        controlsNode, nullptr,
        const_cast<char*>(g_pszKey_CONTROLES));
    if (controlesNode == nullptr)
        return false;

    Cheats_LoadControlBindings_428980(
        reinterpret_cast<S_00692b80*>(controlesNode),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(reinterpret_cast<std::uint8_t*>(self) + 4)),
        32);
    return true;
}

HOOK(0x0050A6C0, S_0050a6c0_LoadControlBindings, "auto");
