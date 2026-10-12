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

// Callees bound by address.
CSF_FUNC(void, __fastcall, C_0082fe88_InitAt24, 0x00425AE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082e598_InstallVtablesAndInitBase, 0x0041E210, (C_0082e598* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting-destructor style teardown of C_0082faf8. Re-establishes the primary
// vtable at +0x00 and the secondary-base vtable at +0x04, resets the embedded
// sub-object at +0x24, runs the TCharTask base teardown, and frees the object
// when bit 0 of the flag argument is set. Returns the object pointer.
static void* __fastcall C_0082faf8_DestroyAndMaybeFree(void* self, void* /*edx*/, char deleteFlag)
{
    // Primary vtable at +0x00 (mov [esi], imm32).
    CSF_AT(void*, self, 0x000) = reinterpret_cast<void*>(static_cast<std::uintptr_t>(CSF_ADDR_VT(0x0082FAF8)));

    // Secondary-base vtable at +0x04 (mov [esi+4], imm32).
    CSF_AT(void*, self, 0x004) = reinterpret_cast<void*>(static_cast<std::uintptr_t>(CSF_ADDR_DATA(0x0082FADC)));

    // Reset the embedded sub-object at +0x24 (address of sub-object, not a pointer load).
    C_0082fe88_InitAt24(reinterpret_cast<uint8_t*>(self) + 0x24, nullptr);

    // Base-class teardown.
    C_0082e598_InstallVtablesAndInitBase(reinterpret_cast<C_0082e598*>(self), nullptr);

    // Free the memory block only when the deleting flag is set.
    if ((deleteFlag & 1) != 0) {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x004240F0, C_0082faf8_DestroyAndMaybeFree, "auto");
