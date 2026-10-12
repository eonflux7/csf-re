#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x6D5920 (__thiscall, two stack args, ret 8).
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResolveResourceWithVariant, 0x006D5920,
         (void* self, void* edx, char* String2, uint32_t* a3));

// Resolves a texture resource name. ecx = this is passed through unchanged, and the
// name argument is forwarded together with a pointer to its own parameter slot.
static unsigned int __fastcall GfxScene_ResolveResource(S_006d4450* self, void* /*edx*/, char* String2)
{
    return GfxScene_ResolveResourceWithVariant(self, nullptr, String2, reinterpret_cast<uint32_t*>(&String2));
}

HOOK(0x006D59B0, GfxScene_ResolveResource, "auto");
