#include <cstdint>
#include "globals.h"
#include "hook.h"

// Calls the callback stored in the record at callbackRecord (fn ptr at +0, user data at +4),
// passing (frame, userData) with cdecl convention, then returns frame unchanged.
static int __cdecl GfxFrames_InvokeCallbackPassThrough(int frame, int callbackRecord)
{
    uint32_t* record = reinterpret_cast<uint32_t*>(callbackRecord);
    auto callback = reinterpret_cast<void(__cdecl*)(int, int)>(record[0]);
    int userData = static_cast<int>(record[1]);
    callback(frame, userData);
    return frame;
}

HOOK(0x006C01E0, GfxFrames_InvokeCallbackPassThrough, "auto");
