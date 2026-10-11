#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedPosition, 0x004BAD20, (void* self, void* edx, int a2, char a3, uint32_t* a4, int a5, char a6));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiCore_CopyAndApplyLinkedPosition(C_0085e7b8* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint32_t linkedBase = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;

    void* sourcePosition = reinterpret_cast<void*>(linkedBase + 1612);
    void* destinationPosition = reinterpret_cast<void*>(
        static_cast<uint32_t>(GetLinkedObjectField10(self, nullptr)) + 16);

    CSF_AT(uint32_t, destinationPosition, 0) =
        CSF_AT(uint32_t, sourcePosition, 0);
    CSF_AT(uint32_t, destinationPosition, 4) =
        CSF_AT(uint32_t, sourcePosition, 4);
    CSF_AT(uint32_t, destinationPosition, 8) =
        CSF_AT(uint32_t, sourcePosition, 8);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(linkedBase + 432));

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;

    uint32_t* position = reinterpret_cast<uint32_t*>(
        static_cast<uint32_t>(GetLinkedObjectField10(self, nullptr)) + 16);
    void* provider = GetCachedObjectProvider(self, nullptr);

    return AiCore_ApplyLinkedPosition(
        reinterpret_cast<void*>(linkedBase + 432),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(provider)),
        1,
        position,
        1120403456,
        1);
}

HOOK(0x004BECF0, AiCore_CopyAndApplyLinkedPosition, "auto");
