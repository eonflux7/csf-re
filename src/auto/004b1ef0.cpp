#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ComputeLinkedTaskFlag, 0x004DB620, (void* self, void* edx, int a1, int a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiCore_UpdateLinkedTask, 0x004B1420, (void* self, void* edx, int a3, int a4, char a5));

static void __stdcall Ai_MacroOrderUpdateLinkedTask(C_0085e7b8* a1, int a2)
{
    AiCoverAttackTasks_UpdateCharacterTasks(reinterpret_cast<int>(a1), a2);

    if ((IsField10Null(a1, nullptr) & 0xFF) == 0)
    {
        int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);

        uint32_t linkedBase = 0;
        if (linkedValue != 0)
            linkedBase = static_cast<uint32_t>(linkedValue) - 4;

        void* cachedProvider = GetCachedObjectProvider(a1, nullptr);
        void* flagSelf = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedBase)) + 0x3D8;
        char linkedFlag = static_cast<char>(
            ComputeLinkedTaskFlag(flagSelf, nullptr, reinterpret_cast<int>(cachedProvider), 1));

        int resolvedValue = ResolveEntryLookupValue(a1, nullptr);
        int linkedField10 = GetLinkedObjectField10(a1, nullptr);
        AiCore_UpdateLinkedTask(reinterpret_cast<void*>(linkedField10), nullptr, resolvedValue, a2, linkedFlag);
    }
}

HOOK(0x004B1EF0, Ai_MacroOrderUpdateLinkedTask, "auto");
