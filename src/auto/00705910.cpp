#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Callee bindings (copied from the packet)
CSF_FUNC(int, __cdecl, RwImageGetPath, 0x00625320, ());
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, GetOptionsByte1, 0x00697320, ());
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

// Builds "<image path><name>" and asks CoreFiles whether that page is available,
// with the options flag 1 temporarily cleared around the check.
static bool __cdecl ParticleRender_CheckImagePageAvailable(const char* imageName, int /*unusedA2*/, int /*unusedA3*/)
{
    char Buffer[0x204];
    memset(Buffer, 0, sizeof(Buffer));

    const char* path = reinterpret_cast<const char*>(static_cast<uintptr_t>(RwImageGetPath()));
    _sprintf(Buffer, CSF_STR(0x849690, "%s%s"), path, imageName);

    char savedFlag1 = GetOptionsByte1();
    Options_SetFlag1(0);

    char pageAvailable = static_cast<char>(CoreFiles_CheckPageAvailability(Buffer, 0));

    Options_SetFlag1(savedFlag1);
    return static_cast<bool>(pageAvailable);
}

HOOK(0x00705910, ParticleRender_CheckImagePageAvailable, "auto");
