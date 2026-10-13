#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, AppendFourByteElement_50CDF0, 0x0050CDF0, (void* self, void* , std::uint32_t* source));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalSlot_ApplyAndClear, 0x006B1660, (void* self, void*));
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(const char*, g_pszMenu_P_SalDem, 0x00899B98);

static char* __fastcall CMenuMainPC_CreateCopyRightStatic(
    CMenuMainPC* self, void* /*edx*/)
{
    auto* globalState =
        reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
    std::uintptr_t result = reinterpret_cast<std::uintptr_t>(
        C_00861120_GetSchedulerProxy(globalState, nullptr)) - 1;

    if (result != 0)
    {
        --result;
        if (result == 0)
            self->field_0038 = 0xFFFFFFFF;
        return reinterpret_cast<char*>(result);
    }

    self->field_0038 = 0xFFFFFFFF;

    if (CSF_AT(std::uint8_t,
               reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0x112) == 0)
    {
        return reinterpret_cast<char*>(self->vInvokeInnerVirtual());
    }

    if (CSF_AT(std::uint32_t,
               reinterpret_cast<void*>(GetGlobalInt()), 0x8C) != 0)
    {
        GlobalSlot_ApplyAndClear(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    }

    alignas(8) std::uint8_t listBytes[0x10];
    S_0050c9f0& list = *reinterpret_cast<S_0050c9f0*>(listBytes);

    alignas(8) std::uint8_t elementBytes[0x4];
    std::uint32_t& element =
        *reinterpret_cast<std::uint32_t*>(elementBytes);

    void* menu = reinterpret_cast<void*>(self->field_0010);
    element = ((int(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, menu, 0)[0x58 / 4])(
            menu, nullptr, g_pszMenu_P_SalDem);

    std::memset(&list.field_0004, 0, 12);
    AppendFourByteElement_50CDF0(&list, nullptr, &element);

    menu = reinterpret_cast<void*>(self->field_0010);
    ((void(__fastcall*)(void*, void*, S_0050c9f0*, int))
        CSF_AT(void**, menu, 0)[0x64 / 4])(
            menu, nullptr, &list, 0);

    char* storage = reinterpret_cast<char*>(list.field_0004);
    if (storage)
        return reinterpret_cast<char*>(Mem_FreeDebug(storage));
    return storage;
}

HOOK(0x00543E60, CMenuMainPC_CreateCopyRightStatic, "auto");
