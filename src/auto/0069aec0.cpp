#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets two state dwords on the loader, asks the child object at +0x14 whether
// the context can begin (vtable slot 3). If it says yes, tail-jumps into the
// child's vtable slot 2 (the actual begin routine). Otherwise returns the probe's result.
static hook::Tail __fastcall SceneLoader_BeginContext(void* self, const uint32_t* /*args*/)
{
    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;

    void* childObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14));
    int probeResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, childObject, 0)[0x0C / 4])(childObject, nullptr);

    // The original tests only the low byte (test al, al) and reloads [self+0x14] afterwards.
    if ((probeResult & 0xFF) != 0)
    {
        void* tailObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14));
        return hook::tail_jump(tailObject, (*reinterpret_cast<void***>(tailObject))[0x08 / 4]);
    }

    return hook::tail_ret(probeResult);
}

HOOK_TAIL(0x0069AEC0, SceneLoader_BeginContext, "auto");
