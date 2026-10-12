#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(unsigned int, __fastcall, CMenuMpSessions_RefreshSessionSearch, 0x00562D50, (uint32_t* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, CMenuMpSessions_ProcessSelectedSession, 0x00562ED0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());

static char __fastcall CMenuMpSessions_UpdateAutomaticSessionSelection(
    CMenuMpSessions* self, void* /*edx*/)
{
    const int globalStateAddress = GetGlobalDword_73C020();
    if (CSF_AT(int, reinterpret_cast<void*>(globalStateAddress), 0x98) == 6)
        return static_cast<char>(globalStateAddress);

    if (self->field_01cc != uint32_t(-1))
        return static_cast<char>(
            CMenuMpSessions_ProcessSelectedSession(self, nullptr));

    char result;
    if (self->field_01e9)
    {
        const uint32_t attemptCount = self->field_01ec + 1;
        self->field_01e9 = 0;
        self->field_01ec = attemptCount;
        self->field_01e8 = 0;
        result = static_cast<char>(attemptCount);

        if (static_cast<int32_t>(attemptCount) > 2)
        {
            SceneLoader_NoOp();
            void* globalState =
                reinterpret_cast<void*>(GetGlobalState_50D190());
            result = static_cast<char>(GameBddLoader_DispatchMenuRequest(
                globalState, nullptr, 1347703090, 1, 3, 0));
            self->field_01e4 = 4;
        }
    }
    else if (self->field_01e8)
    {
        result = 0;
        if (self->field_0070)
        {
            int8_t bestScore = 0;
            for (int row = 0; row < 10; ++row)
            {
                uint8_t score = 0;
                const int metric = GetIndexedTableValue_714C20(
                    reinterpret_cast<int*>(self->field_0070),
                    nullptr, row, 3);
                const int status = GetIndexedTableValue_714C20(
                    reinterpret_cast<int*>(self->field_0070),
                    nullptr, row, 1);

                if (metric != -1 && status != 0)
                {
                    score = static_cast<uint8_t>(21 - metric / 50);
                    if (status > 3 && status < 12)
                        score = static_cast<uint8_t>(score + 10);
                    if (status % 2 != 0)
                        score = static_cast<uint8_t>(score + 5);
                }

                result = static_cast<char>(score);
                if (static_cast<int>(score) > static_cast<int>(bestScore))
                {
                    self->field_01cc = row;
                    bestScore = static_cast<int8_t>(score);
                }
            }

            if (self->field_01cc == uint32_t(-1))
                self->field_01e9 = 1;
        }
    }
    else
    {
        result = static_cast<char>(CMenuMpSessions_RefreshSessionSearch(
            reinterpret_cast<uint32_t*>(self), 0));
    }

    if (self->field_01cc != uint32_t(-1))
        result = static_cast<char>(
            CMenuMpSessions_ProcessSelectedSession(self, nullptr));

    return result;
}

HOOK(0x005637C0, CMenuMpSessions_UpdateAutomaticSessionSelection, "auto");
