#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_GlobalValue_8BB404, 0x008BB404);

// The first three parameters supply EAX, EDX and ECX respectively.
// The remaining four are passed on the stack, with caller cleanup.
using FileSystemHandler = int (__attribute__((regparm(3))) *)( 
    int registerEax, int registerEdx, int registerEcx,
    int fileSystem, int argument2, int argument3, int argument4);

static int __cdecl RtFsManager_ForwardToDefaultFileSystem(
    int /*argument1*/, int argument2, int argument3, int argument4)
{
    const int fileSystem = g_GlobalValue_8BB404;
    const auto handler = CSF_AT(
        FileSystemHandler, reinterpret_cast<void*>(fileSystem), 0x28);

    return handler(
        fileSystem, argument2, argument3,
        fileSystem, argument2, argument3, argument4);
}

HOOK(0x0069FA20, RtFsManager_ForwardToDefaultFileSystem, "auto");
