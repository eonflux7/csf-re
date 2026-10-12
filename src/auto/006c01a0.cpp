#include "hook.h"

// Calls the callback with the frame pointer, then returns the same frame pointer.
static int __cdecl ApplyFrameCallbackAndReturnFrame(int frame, void (__cdecl* callback)(int)) {
    callback(frame);
    return frame;
}

HOOK(0x006C01A0, ApplyFrameCallbackAndReturnFrame, "auto");
