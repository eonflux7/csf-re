#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(bool, __fastcall, NetProfile_CheckAccountName, 0x005184B0, (void* self, void* , const char* accountName, char expectedLeadByte));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

static void __fastcall S_0055c780_SubmitAccountName(
    S_0055c780* self, void* /*edx*/)
{
    self->field_0280 = 1;
    if (g_MpNetworkType == 2)
    {
        self->field_0280 = 5;
        return;
    }

    void* profileManager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* profile = CSF_AT(void*, profileManager, 0x14);
    uint8_t* accountName = &self->field_003c;

    if (NetProfile_CheckAccountName(
            profile, nullptr, reinterpret_cast<const char*>(accountName), 0))
    {
        self->field_0280 = 3;
        void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(loader, nullptr, 1229992241, 1, 1, 0);
    }
    else
    {
        self->field_0280 = 5;
        void* networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        ((void(__fastcall*)(void*, void*, uint8_t*, uint8_t*, uint8_t*))
            CSF_AT(void**, networkState, 0)[0x28 / 4])(
                networkState, nullptr, accountName,
                &CSF_AT(uint8_t, self, 0x5D), &g_byte_82E65C);

        alignas(8) uint8_t menuBuffer[0xB4];
        MenuBuffer_Init(menuBuffer, nullptr);
        menuBuffer[24] = 1;

        void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(
            loader, nullptr, 1095053618, 1, 0,
            reinterpret_cast<int>(menuBuffer));

        SceneLoader_DestroyMenuBuffer(
            reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
    }
}

HOOK(0x0055CE90, S_0055c780_SubmitAccountName, "auto");
