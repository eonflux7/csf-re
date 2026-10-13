#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: fills an out-pointer from the registry block (3 stack args, ret 12).
CSF_FUNC(void**, __fastcall, SceneLoader_QueryRegistryBlock, 0x0069ABF0, (void* self, void* edx, void** a2, int* a3, int* a4));
// Original game-heap free (cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Vtable reference listed in the packet for the 0x69ABF0 call site. Kept as a file-local, side-effect-free binding.
static const void* const kS_0069abf0VtblRef = CSF_VTBL(0x0069ABF0);

// Destroys the reference registry block held at +0x004 and clears the block pointer and count (+0x008).
static char __fastcall SceneLoader_DestroyReferenceRegistry(S_0069abf0* self, void* /*edx*/)
{
    // Block pointer as loaded first by the original.
    uint32_t blockPtr = self->field_0004;
    // First dword of the block is passed as the third argument.
    int* firstWord = *reinterpret_cast<int**>(blockPtr);

    // Out-value written by the callee; the original leaves this stack slot uninitialized.
    void* outValue = nullptr;
    SceneLoader_QueryRegistryBlock(self, nullptr, reinterpret_cast<void**>(&outValue), firstWord,
                                   reinterpret_cast<int*>(blockPtr));

    // Re-read the field after the call, as the original does.
    uint32_t currentBlock = self->field_0004;
    char result = static_cast<char>(currentBlock);
    if (currentBlock != 0) {
        result = static_cast<char>(Mem_FreeDebug(reinterpret_cast<void*>(currentBlock)));
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    return result;
}

HOOK(0x69ACF0, SceneLoader_DestroyReferenceRegistry, "auto");
