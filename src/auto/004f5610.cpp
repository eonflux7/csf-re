#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_004f5420_ApplyProvider, 0x004F54A0,
         (void* self, void* edx, int a2, char a3, char a4, char a5));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetArrayElementAt38, 0x004FBFC0,
         (void* self, void* , int index));

static char __fastcall S_004f5420_SetProviderState(
    S_004f5420* self, void* /*edx*/, int state)
{
    if (self->field_0008)
        return static_cast<char>(self->field_0008);

    int priority = -1;
    self->field_0020 = 0;

    switch (state)
    {
    case 0:
        if (self->field_0018)
            priority = 0;
        break;
    case 1:
    case 2:
    case 3:
    case 7:
        priority = 1;
        break;
    case 4:
    case 5:
        priority = 2;
        break;
    case 6:
        self->field_0020 = 1;
        priority = 3;
        break;
    default:
        break;
    }

    void* owner = reinterpret_cast<void*>(self->field_0010);
    void* provider =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x68 / 4])(
            owner, nullptr);
    char result = static_cast<char>(reinterpret_cast<std::uintptr_t>(provider));

    if (priority != 0 &&
        priority < static_cast<std::int32_t>(self->field_001c))
    {
        result =
            ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, provider, 0)[0x40 / 4])(
                provider, nullptr, 0);
        if (!result)
            return result;
    }

    self->field_001c = priority;

    char useProvider;
    switch (state)
    {
    case 0:
        return S_004f5420_ApplyProvider(self, nullptr, -1, 1, 0, 0);
    case 1:
    case 2:
    case 3:
    case 7:
        useProvider = 1;
        break;
    case 4:
    case 5:
    case 6:
        useProvider = 0;
        break;
    default:
        return result;
    }

    void* registry = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    int entry = GetArrayElementAt38(registry, nullptr, state);
    return S_004f5420_ApplyProvider(self, nullptr, entry, useProvider, 0, 0);
}

HOOK(0x004F5610, S_004f5420_SetProviderState, "auto");
