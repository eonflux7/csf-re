#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// int __cdecl sub_7C61A0(int a1)
CSF_FUNC(int, __cdecl, RtFSManagerSetDefaultFileSystem, 0x7C61A0, (int fileSystem));

// 0x0069FDC0: if the object holds a file system pointer, make it the FS manager's default one.
static int __fastcall C_0085ec18_ApplyStoredDefaultFileSystem(C_0085ec18* self, void* /*edx*/)
{
    // The original loads [ecx+4] exactly once, before the call.
    const uint32_t fileSystem = self->field_0004;
    if (fileSystem)
        return RtFSManagerSetDefaultFileSystem(static_cast<int>(fileSystem));

    return static_cast<int>(fileSystem);
}

HOOK(0x0069FDC0, C_0085ec18_ApplyStoredDefaultFileSystem, "auto");
