#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, ResolveEmbeddedSubobjectHandler, 0x004E2190, (void* self, void* edx, char a2));

// Original: sub ecx, 4 ; jmp 0x4E2190
// Adjusts `this` back to the enclosing object (the embedded sub-object sits 4 bytes in)
// and tail-forwards the call with the same stack argument.
static void* __fastcall ForwardToEnclosingObjectHandler(char* self, void* /*edx*/, char a2) {
    return ResolveEmbeddedSubobjectHandler(self - 4, nullptr, a2);
}

HOOK(0x004E2180, ForwardToEnclosingObjectHandler, "auto");
