#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using BinkPauseImportFn = int(__fastcall*)(
    void* receiver, void* edx, void* bink, int pause);

CSF_GLOBAL(BinkPauseImportFn, g_BinkPauseImport, 0x82D394);

static int __fastcall C_00861a00_PauseVideoPlayback(
    C_00861a00* self, void* /*edx*/, uint8_t pauseFlag)
{
    void* bink = reinterpret_cast<void*>(self->field_000c);
    return g_BinkPauseImport(bink, nullptr, bink, static_cast<int>(pauseFlag));
}

HOOK(0x702BC0, C_00861a00_PauseVideoPlayback, "auto");
