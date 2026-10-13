#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IsField0050Null, 0x0040DA80, (void* self, void*));
CSF_FUNC(int, __fastcall, EntryLookup_FindEntryWhenState1, 0x0040DA90, (void* self, void*));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, AiAttack_IsStoredValueNegative, 0x0047AEB0, (float* self, void*));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlEntryByIndex, 0x0049ED10, (C_008342e8* self, void* , int index));
CSF_FUNC(uint32_t*, __fastcall, C_0083dd58_SetCandidatePosition, 0x004CDE10, (void* self, void* edx, int a3));
CSF_FUNC(std::uint32_t*, __fastcall, C_0083dd58_FindCompatibleCandidate, 0x004D0750, (C_0083dd58* self, void* , int selectionContext));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083dd58_UpdateCompatibleTarget(
    C_0083dd58* self, void* /*edx*/, int /*unused*/)
{
    auto* entry = reinterpret_cast<C_0082dd78*>(
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0014), 0x4C));

    if (static_cast<std::uint8_t>(IsField0050Null(entry, nullptr)) != 0)
    {
        std::uint32_t* candidate = nullptr;

        if (CSF_AT(std::uint8_t, self, 0xE4) != 0)
        {
            auto* currentEntry = reinterpret_cast<C_0082dd78*>(
                CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0014), 0x4C));

            if (reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr) != 0)
            {
                if ((currentEntry->field_0034 & 4) == 0)
                    candidate = C_0083dd58_FindCompatibleCandidate(self, nullptr, 14);
            }
        }

        if (CSF_AT(std::uint8_t, self, 0xE5) != 0)
        {
            EntryLookup_FindEntryWhenState1(entry, nullptr);
            CSF_AT(std::uint8_t, self, 0xE5) = 0;
        }

        if (candidate != nullptr)
        {
            if (static_cast<std::uint8_t>(AiAttack_IsStoredValueNegative(
                    reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(self) + 0x60),
                    nullptr)) == 0)
            {
                C_0083dd58_SetCandidatePosition(
                    self, nullptr, reinterpret_cast<int>(candidate + 7));
                CSF_AT(std::uint8_t, self, 0xE6) = 0;
            }
        }
        else if (CSF_AT(std::uint8_t, self, 0xE6) == 0 &&
                 reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
                     CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr) != 0)
        {
            auto* owner = reinterpret_cast<C_008342e8*>(self->field_0014);
            void* scene = g_pCurrentScene;
            int handle = C_008342e8_GetControlEntryByIndex(owner, nullptr, 0);
            void* control = reinterpret_cast<void*>(CHandleTable_Resolve(scene, nullptr, handle));

            reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
                CSF_AT(void**, control, 0)[0x8C / 4])(
                    control, nullptr, 0, 0, -1, 0);
            CSF_AT(std::uint8_t, self, 0xE6) = 1;
        }
    }

    std::uint8_t result = CSF_AT(std::uint8_t, self, 0xE4);
    if (result != 0)
    {
        result = reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
    }

    if (result == 0)
        self->field_0074 = 1;

    return static_cast<char>(result);
}

HOOK(0x004CE040, C_0083dd58_UpdateCompatibleTarget, "auto");
