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

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(CGuiBuilder*, __fastcall, CGuiBuilder_Construct, 0x0071F3F0, (CGuiBuilder* self, void* edx, int owner, char flag, int variant));

// Builds a CGuiBuilder (136 bytes) from the global value and resolved menu object.
static CGuiBuilder* CreateGuiBuilderFromGlobals()
{
    CGuiBuilder* builder = reinterpret_cast<CGuiBuilder*>(Mem_Alloc(136));
    if (!builder)
        return nullptr;

    char flag = static_cast<char>(CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8B056C())), 0x0C));
    void* globalObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
    int owner = static_cast<int>(reinterpret_cast<uintptr_t>(MenuControls_ResolveObject(globalObject, nullptr)));
    return CGuiBuilder_Construct(builder, nullptr, owner, flag, 0);
}

HOOK(0x511C10, CreateGuiBuilderFromGlobals, "auto");
