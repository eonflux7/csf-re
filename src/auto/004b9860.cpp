#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(void, __fastcall, S_0048c9f0_CopyTargetHandle, 0x0048C9F0, (S_0048c9f0* self, void* , const void* handle));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(void, __fastcall, C_0083eb70_AddEventRecord, 0x004E87D0, (C_0083eb70* self, void* , int a2));

// Additional vtable bindings listed by the packet's address catalog.
[[maybe_unused]] static const auto k_TaskTargetBufferVtable = CSF_VTBL(0x0047AE10);
[[maybe_unused]] static const auto k_TargetHandleCopyVtable = CSF_VTBL(0x0048C9F0);

static void __fastcall AiDistracted_HandleExpiredDistraction(
    void* self, void* /*edx*/)
{
    auto* bytes = static_cast<std::uint8_t*>(self);

    char* targetName = AiCore_GetTaskTargetNameBuffer(
        CSF_AT(S_0047ae10*, self, 0x718), nullptr);
    ResetState(targetName, nullptr);

    void* context = CSF_AT(void*, self, 0x2C);
    alignas(8) std::uint8_t emptyHandle[0xC];
    CSF_AT(void**, emptyHandle, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(int, emptyHandle, 0x4) = -1;
    CSF_AT(int, emptyHandle, 0x8) = -1;
    S_0048c9f0_CopyTargetHandle(
        CSF_AT(S_0048c9f0*, context, 0x204), nullptr, emptyHandle);

    const int distractionType = CSF_AT(int, self, 0x608);
    if (distractionType == 1 || distractionType == 9)
    {
        const int linkedValue = GetLinkedObjectValue_4DB5C0(bytes + 8, nullptr);
        void* linkedObject = linkedValue
            ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linkedValue) - 4)
            : nullptr;

        void* linkedContext = CSF_AT(void*, linkedObject, 0x2C);
        const int eventIndex = CSF_AT(int, self, 0x630);
        auto* eventManager = CSF_AT(C_0083eb70*, linkedContext, 0x3C);
        void* eventRecord = bytes + 0x628;
        if (eventIndex < 0)
            eventRecord = bytes + 0x610;

        C_0083eb70_AddEventRecord(
            eventManager, nullptr, reinterpret_cast<int>(eventRecord));
    }

    CSF_AT(int, self, 0x604) = -1;
    CSF_AT(int, self, 0x608) = -1;
    CSF_AT(int, self, 0x60C) = -1;
    CSF_AT(int, self, 0x614) = -1;
    CSF_AT(int, self, 0x618) = -1;
    CSF_AT(int, self, 0x62C) = -1;
    CSF_AT(int, self, 0x630) = -1;
    CSF_AT(int, self, 0x638) = 0;
    CSF_AT(float, self, 0x63C) = 1.0f;
    CSF_AT(int, self, 0x670) = 0;
    CSF_AT(std::uint8_t, self, 0x684) = 0;
    CSF_AT(std::uint8_t, self, 0x685) = 0;
    CSF_AT(int, self, 0x67C) = 0;
    CSF_AT(int, self, 0x680) = 0;
    CSF_AT(float, self, 0x678) = FLT_MAX;
}

HOOK(0x004B9860, AiDistracted_HandleExpiredDistraction, "auto");
