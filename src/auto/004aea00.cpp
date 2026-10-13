#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Cover-attack task teardown step run before the object is released (0x4AE760).
CSF_FUNC(int, __fastcall, AiCore_DestroyCoverAttackTask, 0x004AE760, (void* self, void* edx));

// Game heap free (0x691110).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for TFncAtacarDesdeParapeto: runs the cover-attack task
// cleanup, then frees the object through the game heap when the low bit of the
// flags argument is set. Returns the object pointer, as the original does.
static char* __fastcall TFncAtacarDesdeParapeto_Destroy(TFncAtacarDesdeParapeto* self, void* /*edx*/, char flags)
{
    AiCore_DestroyCoverAttackTask(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<char*>(self);
}

HOOK(0x4AEA00, TFncAtacarDesdeParapeto_Destroy, "auto");
