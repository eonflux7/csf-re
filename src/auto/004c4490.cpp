#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* /*edx*/, int handle));
CSF_FUNC(std::uint32_t*, __fastcall, S_004bdcd0_AppendRecord, 0x004C37F0, (S_004bdcd0* self, void* /*edx*/, std::uint32_t* source, int capacityHint));
CSF_FUNC(int, __fastcall, AiGoToCover_TryPrimaryCover, 0x004C42C0, (void* self, void* /*edx*/, int a2));
CSF_FUNC(int, __fastcall, AiGoToCover_TryAlternateCover, 0x004C43B0, (void* self, void* /*edx*/, int a2));

static int __fastcall AiGoToCover_TryAssignCover(S_004bd310* self, void* /*edx*/, std::uint32_t* a2)
{
    self->field_001c = 0;

    // Default: a failed precondition (negative a2[5] or unresolved handle) appends the record.
    bool appendRecord = true;
    if (static_cast<std::int32_t>(a2[5]) >= 0 &&
        CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<std::intptr_t>(a2 + 3))) != 0)
    {
        bool assigned;
        if (a2[1] == 0xFFFFFFFFu)
        {
            if (a2[2] == 0xFFFFFFFFu)
            {
                // Original returns here without appending.
                return static_cast<std::int32_t>(a2[11]) >= 0 ? 1 : 0;
            }
            // The original tests only al of the callee's result.
            int alternateResult = AiGoToCover_TryAlternateCover(self, nullptr,
                static_cast<int>(reinterpret_cast<std::intptr_t>(a2)));
            assigned = (static_cast<std::uint8_t>(alternateResult) != 0);
        }
        else
        {
            int primaryResult = AiGoToCover_TryPrimaryCover(self, nullptr,
                static_cast<int>(reinterpret_cast<std::intptr_t>(a2)));
            assigned = (static_cast<std::uint8_t>(primaryResult) != 0);
        }
        appendRecord = assigned;
    }

    if (appendRecord)
    {
        S_004bdcd0_AppendRecord(reinterpret_cast<S_004bdcd0*>(reinterpret_cast<std::uint8_t*>(self) + 4),
                                nullptr, a2, 0);
    }

    return static_cast<std::int32_t>(a2[11]) >= 0 ? 1 : 0;
}

HOOK(0x004C4490, AiGoToCover_TryAssignCover, "auto");
