#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, NetProfile_ConstructObject, 0x00518F00, (void* self, void* edx, int a2));

CSF_GLOBAL(int, g_Value, 0x008B0580);

static uint32_t* __cdecl NetProfile_GetOrCreateObject(int initArg)
{
    (void)CSF_VTBL(0x00518F00);

    uint32_t* result = reinterpret_cast<uint32_t*>(g_Value);
    if (!result)
    {
        void* storage = Mem_Alloc(28);
        if (storage)
        {
            result = NetProfile_ConstructObject(storage, nullptr, initArg);
            g_Value = static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
        }
        else
        {
            g_Value = 0;
        }
    }
    return result;
}

HOOK(0x00518F50, NetProfile_GetOrCreateObject, "auto");
