#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

static int __fastcall S_0048e570_ActivateFocusEntry(
    S_0048e570* self, void* /*edx*/, int* entry)
{
    void* owner = reinterpret_cast<std::uint8_t*>(self) - 12;
    void* focus = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[40 / 4])(owner, nullptr);

    SceneLoader_ConfigureFocusEntry(owner, nullptr, entry);

    void* transform = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, focus, 0)[24 / 4])(focus, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, transform, 0)[76 / 4])(transform, nullptr, entry + 3);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, focus, 0)[72 / 4])(focus, nullptr, 1);

    void* notifier = CSF_AT(void*, self, 0x28);
    if (notifier)
    {
        return reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, notifier, 0)[140 / 4])(notifier, nullptr);
    }
    return 0;
}

HOOK(0x0048E570, S_0048e570_ActivateFocusEntry, "auto");
