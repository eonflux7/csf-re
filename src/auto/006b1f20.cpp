#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, sub_6B1B80, 0x006B1B80, (void* self, void* edx, int a2, int a3, uint8_t* a4));

CSF_GLOBAL(C_00860130*, g_Value_8BB450, 0x008BB450);

static char __cdecl AppMain_InitializeVideo(int initArg0, int initArg1)
{
    // Already initialized: report success without touching anything.
    if (g_Value_8BB450)
        return 1;

    uint8_t initFlag = 0;
    void* memory = Mem_Alloc(0x90);
    if (memory)
    {
        g_Value_8BB450 = reinterpret_cast<C_00860130*>(
            sub_6B1B80(memory, nullptr, initArg0, initArg1, &initFlag));
    }
    else
    {
        g_Value_8BB450 = nullptr;
    }
    return static_cast<char>(initFlag);
}

HOOK(0x006B1F20, AppMain_InitializeVideo, "auto");
