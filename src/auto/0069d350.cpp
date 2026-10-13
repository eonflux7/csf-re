#include <cstdint>
#include "globals.h"
#include "hook.h"

using IsBadReadPtrFn = int(__stdcall*)(const void* ptr, unsigned int ucb);
CSF_GLOBAL(IsBadReadPtrFn, g_IsBadReadPtr, 0x82D244);

static int __cdecl CErrorLog_WalkFrameChain(int outArray, unsigned int count, void** frame)
{
    int result = static_cast<int>(count);
    unsigned int index = 0;
    uint32_t* out = reinterpret_cast<uint32_t*>(outArray);

    if (count == 0)
    {
        out[-1] = 0;
        return result;
    }

    while (reinterpret_cast<uint32_t>(frame) >= 0xFFFFu)
    {
        if (reinterpret_cast<uint32_t>(frame) > 0x7FFEFFFFu)
            break;
        if ((reinterpret_cast<uint32_t>(frame) & 3u) != 0)
            break;

        result = g_IsBadReadPtr(frame, 8u);
        if (result)
            break;

        out[index] = reinterpret_cast<uint32_t>(frame[1]);
        result = static_cast<int>(count);
        frame = reinterpret_cast<void**>(frame[0]);

        if (++index >= count)
        {
            out[count - 1] = 0;
            return result;
        }
    }

    out[index] = 0;
    out[count - 1] = 0;
    return result;
}

HOOK(0x69D350, CErrorLog_WalkFrameChain, "auto");
