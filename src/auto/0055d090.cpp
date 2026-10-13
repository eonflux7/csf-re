#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void* __fastcall CMenuAccountEdit_HandleAccountSaveStatus(
    CMenuAccountEdit* self, void* /*edx*/, int /*unused*/)
{
    auto* globalState = reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
    void* result = C_00861120_GetSchedulerProxy(globalState, nullptr);

    if (reinterpret_cast<std::uintptr_t>(result) == 4)
    {
        self->field_0280 = 5;

        void* accountService = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*, uint8_t*, uint8_t*)>(
            CSF_AT(void**, accountService, 0)[0x28 / 4])(
                accountService, nullptr,
                reinterpret_cast<uint8_t*>(self) + 0x3C,
                reinterpret_cast<uint8_t*>(self) + 0x5D,
                &g_byte_82E65C);

        alignas(8) uint8_t menuBufferBytes[0xB4];
        MenuBuffer_Init(menuBufferBytes, nullptr);
        menuBufferBytes[24] = 1;

        void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(
            loader, nullptr, 1095053618, 1, 0,
            reinterpret_cast<int>(menuBufferBytes));

        return reinterpret_cast<void*>(SceneLoader_DestroyMenuBuffer(
            reinterpret_cast<std::uint32_t*>(menuBufferBytes), nullptr));
    }

    if (reinterpret_cast<std::uintptr_t>(result) == 8)
        self->field_0280 = 1;

    return result;
}

HOOK(0x0055D090, CMenuAccountEdit_HandleAccountSaveStatus, "auto");
