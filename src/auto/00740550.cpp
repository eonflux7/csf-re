#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0064B250: method that expects the *enclosing* object in ecx and pops no stack bytes.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x64B250, (void* self, void* edx));

// 0x00740550: the incoming ecx points at a sub-object embedded at offset 0x78 inside the
// enclosing object. The thunk rebases ecx to the enclosing object's address (ecx - 0x78)
// and tail-jumps into the real method with no other arguments, returning its result.
static int __fastcall ForwardToEnclosingObjectMethod(void* self, void* /*edx*/)
{
    char* enclosingObject = static_cast<char*>(self) - 0x78;
    return Sub_64B250(enclosingObject, nullptr);
}

HOOK(0x740550, ForwardToEnclosingObjectMethod, "auto");
