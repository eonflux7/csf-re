#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, CoreFiles_ConstructOptions, 0x00698270, (void* self, void* edx, char a2));

CSF_GLOBAL(char*, g_pOptions, 0x008BAF8C);

static char* __cdecl CoreFiles_GetOrCreateOptions(char initializationFlag, int /*unused*/)
{
    (void)CSF_VTBL(0x00698270);

    char* options = g_pOptions;
    if (!options)
    {
        void* storage = Mem_Alloc(108);
        if (storage)
        {
            options = reinterpret_cast<char*>(
                CoreFiles_ConstructOptions(storage, nullptr, initializationFlag));
            g_pOptions = options;
        }
        else
        {
            options = nullptr;
            g_pOptions = nullptr;
        }
    }
    return options;
}

HOOK(0x00698310, CoreFiles_GetOrCreateOptions, "auto");
