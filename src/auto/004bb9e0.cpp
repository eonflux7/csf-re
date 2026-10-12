#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedPosition, 0x004BAD20, (void* self, void* edx, int a2, char a3, uint32_t* a4, int a5, char a6));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall C_0085e7b8_ApplyLinkedPosition(C_0085e7b8* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint32_t linkedBase = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(linkedBase + 432));

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;

    int linkedField = GetLinkedObjectField10(self, nullptr);
    uint32_t* position = AiCoverAttackTasks_GetPositionData(
        reinterpret_cast<void*>(static_cast<uint32_t>(linkedField) + 48), nullptr);
    void* provider = GetCachedObjectProvider(self, nullptr);

    return AiCore_ApplyLinkedPosition(
        reinterpret_cast<void*>(linkedBase + 432), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(provider)), 1, position, 0, 0);
}

HOOK(0x004BB9E0, C_0085e7b8_ApplyLinkedPosition, "auto");
