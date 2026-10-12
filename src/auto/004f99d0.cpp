#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_Value8B055C, 0x008B055C);

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, DevConsole_Construct, 0x004F9750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DevConsole_Initialize, 0x004F9670, (void* self, void* edx));

static char __cdecl DevConsole_EnsureInitialized()
{
    if (!g_Value8B055C)
    {
        void* storage = Mem_Alloc(268);
        uint32_t* console = storage ? DevConsole_Construct(storage, nullptr) : nullptr;
        g_Value8B055C = static_cast<int>(reinterpret_cast<std::uintptr_t>(console));
        DevConsole_Initialize(console, nullptr);
    }
    return 1;
}

HOOK(0x004F99D0, DevConsole_EnsureInitialized, "auto");
