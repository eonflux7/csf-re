#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, GfxObjectInstances_AppendMaterialReference, 0x006C1C00, (void* self, void* edx, uint32_t* a3));

static int __cdecl GfxTexDict_AppendMaterialRefReturnArg(int a1, int a2)
{
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));
    GfxObjectInstances_AppendMaterialReference(owner, nullptr, reinterpret_cast<uint32_t*>(&a1));
    return a1;
}

HOOK(0x006C1C70, GfxTexDict_AppendMaterialRefReturnArg, "auto");
