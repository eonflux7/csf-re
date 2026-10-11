#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0,
         (void* self, void* edx, int elapsed));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, TCharTask_ShouldRefreshAttackTarget, 0x0047C020,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiAttackTasks_UpdateTarget, 0x0047C3E0,
         (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static void __fastcall TCharTask_UpdateAttackTarget(
    TCharTask* self, void* /*edx*/, int elapsed)
{
    // Retain the vtable binding explicitly listed in the packet.
    (void)CSF_VTBL(0x0047AE10);

    TCharTask_BaseUpdate(self, nullptr, elapsed);

    void* character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    int state = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, character, 0)[0x30 / 4])(character, nullptr);

    if (state != 8)
    {
        character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
        void* interfaceObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr);
        void* statusObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, interfaceObject, 0)[0x68 / 4])(interfaceObject, nullptr);

        if (reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, statusObject, 0)[0x44 / 4])(statusObject, nullptr, 0))
            return;
    }

    character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    state = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, character, 0)[0x30 / 4])(character, nullptr);

    if (state != 8)
    {
        character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, character, 0)[0x8C / 4])(character, nullptr, 8, 0, -1, 0);
    }

    character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    void* interfaceObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr);
    void* statusObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, interfaceObject, 0)[0x68 / 4])(interfaceObject, nullptr);

    if (reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, statusObject, 0)[0x44 / 4])(statusObject, nullptr, 0)
        && !TCharTask_ShouldRefreshAttackTarget(self, nullptr))
        return;

    int targetTask = CharTask_LookupByOffset(
        self, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 60));
    char* targetNameBuffer =
        AiCore_GetTaskTargetNameBuffer(CSF_AT(void*, self, 0x08), nullptr);
    AiAttackTasks_UpdateTarget(
        reinterpret_cast<void*>(static_cast<uintptr_t>(targetTask)),
        nullptr, targetNameBuffer + 16);
}

HOOK(0x0047C5E0, TCharTask_UpdateAttackTarget, "auto");
