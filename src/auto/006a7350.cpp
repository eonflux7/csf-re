#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_GlobalState, 0x008BB418);

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0085fe28_ConstructNetworkAccount, 0x006A7060,
         (void* self, void* edx));

static int __cdecl NetGatheringDdl_PrepareNetworkAccount()
{
    void* account = reinterpret_cast<void*>(g_GlobalState);
    if (account)
    {
        if (!((bool(__fastcall*)(void*, void*))
              CSF_AT(void**, account, 0)[4])(account, nullptr))
        {
            return g_GlobalState != 0;
        }

        account = reinterpret_cast<void*>(g_GlobalState);
        if (account &&
            ((bool(__fastcall*)(void*, void*))
             CSF_AT(void**, account, 0)[4])(account, nullptr))
        {
            account = reinterpret_cast<void*>(g_GlobalState);
            if (account)
            {
                ((void(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, account, 0)[0])(account, nullptr, 1);
                g_GlobalState = 0;
            }
        }
    }

    void* storage = Mem_Alloc(832);
    if (!storage)
    {
        g_GlobalState = 0;
        return 0;
    }

    uint32_t* constructed =
        C_0085fe28_ConstructNetworkAccount(storage, nullptr);
    g_GlobalState = static_cast<int>(
        reinterpret_cast<std::uintptr_t>(constructed));

    if (!constructed)
        return 0;

    if (!((bool(__fastcall*)(void*, void*))
          CSF_AT(void**, constructed, 0)[1])(constructed, nullptr))
    {
        account = reinterpret_cast<void*>(g_GlobalState);
        if (account)
        {
            ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, account, 0)[0])(account, nullptr, 1);
            g_GlobalState = 0;
        }
        return 0;
    }

    return g_GlobalState != 0;
}

HOOK(0x006A7350, NetGatheringDdl_PrepareNetworkAccount, "auto");
