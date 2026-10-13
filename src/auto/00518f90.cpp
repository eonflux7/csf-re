#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_Value, 0x008B0580);

CSF_FUNC(int, __fastcall, NetProfile_FooterValueDetach, 0x00518D10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetProfile_FooterValueFinalize, 0x00518E70, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int NetProfile_ReleaseFooterValue()
{
    int result = 0;

    if (g_Value)
    {
        result = NetProfile_FooterValueDetach(
            reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value)), nullptr);

        if (g_Value)
        {
            int valueHandle = g_Value;
            void* valueBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(valueHandle));

            NetProfile_FooterValueFinalize(valueBlock, nullptr);
            result = Mem_Free(valueBlock);
            g_Value = 0;
        }
    }

    return result;
}

HOOK(0x00518F90, NetProfile_ReleaseFooterValue, "auto");
