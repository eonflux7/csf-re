#include <cstdint>
#include <cstring>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiDistracted_UpdateResolvedEntry, 0x004B3D30, (int arg0, float a3, float a4));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));

static void __stdcall AiAlert_UpdateCharacterTasks(C_0085e760* character, float deltaTime)
{
    int deltaTimeBits;
    std::memcpy(&deltaTimeBits, &deltaTime, sizeof(deltaTimeBits));

    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(character), deltaTimeBits);

    if (static_cast<std::uint8_t>(IsField10Null(character, nullptr)) == 0)
    {
        const int entry = ResolveEntryLookupValue(character, nullptr);
        GetLinkedObjectField10(character, nullptr);
        AiDistracted_UpdateResolvedEntry(entry, deltaTime, 0.0f);
    }
}

HOOK(0x004B4850, AiAlert_UpdateCharacterTasks, "auto");
