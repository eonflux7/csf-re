#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __stdcall, AiAlert_ApplyCoverAttack, 0x004ABF50, (int a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCore_CopyCoverAttackState, 0x004D16C0, (void* self, void* , uint32_t* source));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiAlert_ActivateCoverAttack(C_0085e7b8* self)
{
    // Preserve the packet's vtable annotation for the target-name provider.
    (void)CSF_VTBL(0x0047AE10);

    uint32_t linkedAddress =
        static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(self, nullptr));
    uint32_t objectAddress = linkedAddress ? linkedAddress - 4 : 0;

    if (CSF_AT(int32_t, reinterpret_cast<void*>(objectAddress), 0x608) != -1 ||
        CSF_AT(int32_t, reinterpret_cast<void*>(objectAddress), 0x60C) != -1)
    {
        linkedAddress =
            static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(self, nullptr));
        const uint32_t coverObjectAddress = linkedAddress ? linkedAddress - 4 : 0;

        linkedAddress =
            static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(self, nullptr));
        objectAddress = linkedAddress ? linkedAddress - 4 : 0;

        auto* targetProvider = reinterpret_cast<S_0047ae10*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(objectAddress), 0x718));
        char* targetBuffer = AiCore_GetTaskTargetNameBuffer(targetProvider, nullptr);
        AiCore_CopyCoverAttackState(
            targetBuffer, nullptr,
            reinterpret_cast<uint32_t*>(coverObjectAddress + 0x64C));
    }

    linkedAddress =
        static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(self, nullptr));
    objectAddress = linkedAddress ? linkedAddress - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(objectAddress + 0x3D8));

    linkedAddress =
        static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(self, nullptr));
    objectAddress = linkedAddress ? linkedAddress - 4 : 0;

    void* stateProvider = GetCachedObjectProvider(self, nullptr);
    CSF_AT(uint8_t, reinterpret_cast<void*>(objectAddress), 0x400) = 1;

    alignas(8) uint8_t optionsBytes[0x10] = {};
    optionsBytes[1] = 1;
    CSF_AT(uint32_t, optionsBytes, 0x4) = 0;
    CSF_AT(uint32_t, optionsBytes, 0x8) = 0;
    CSF_AT(uint32_t, optionsBytes, 0xC) = 0;
    optionsBytes[0] = 0;

    CoverAttack_SetState_4DC150(
        stateProvider, nullptr, static_cast<int>(objectAddress + 0x3DC));

    void* cachedObject = GetCachedObjectProvider(stateProvider, nullptr);
    return AiAlert_ApplyCoverAttack(
        static_cast<int>(reinterpret_cast<uintptr_t>(cachedObject)),
        reinterpret_cast<uint32_t*>(optionsBytes));
}

HOOK(0x004B42E0, AiAlert_ActivateCoverAttack, "auto");
