#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __stdcall, MatchesTaggedOrPlainKey, 0x0045BAF0, (int keyArg, int recordArg));
CSF_FUNC(char, __fastcall, IsTrackedHandleActive, 0x0045BDA0, (S_0045bda0* self, void*));
CSF_FUNC(int, __fastcall, CopyStateFromRecord, 0x00463FA0, (S_0045bda0* self, void* , int a2));
CSF_FUNC(char, __fastcall, S_00467cc0_ExecuteAction, 0x00467BC0,
         (S_00467cc0* self, void* edx, int16_t* action, C_008313f8* definition));
CSF_FUNC(char, __fastcall, Sub467CC0, 0x00467CC0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char, __fastcall, S_00467cc0_StartActions, 0x004681B0, (void* a1));
CSF_FUNC(void*, __cdecl, GetGlobalBlock8AFDA0, 0x0046ED80, ());
CSF_FUNC(int, __stdcall, GetDwordField4, 0x0046ED90, (int a1));

static char __fastcall S_0045bda0_HandleEventRecord(
    S_0045bda0* self, void* /*edx*/, uint32_t* record)
{
    const char active = IsTrackedHandleActive(self, nullptr);
    if (!active)
        return active;

    if (record[1] == 1 && static_cast<int32_t>(record[4]) >= 0) {
        const uint32_t trackedHandle = self->field_0008;
        if (trackedHandle != record[3])
            return static_cast<char>(trackedHandle);
    }

    const uint32_t state = self->field_0018;
    if (state) {
        if (state != 2)
            return static_cast<char>(state - 2);

        const bool matches = MatchesTaggedOrPlainKey(
            static_cast<int>(self->field_006c),
            static_cast<int>(reinterpret_cast<uintptr_t>(record)));
        if (!matches)
            return static_cast<char>(matches);

        CopyStateFromRecord(
            self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(record)));
        return Sub467CC0(self, nullptr, self->field_001c);
    }

    void* script = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    const uint32_t begin = CSF_AT(uint32_t, script, 0x0C);
    uint32_t eventCount = 0;
    if (begin) {
        const uint32_t byteCount = CSF_AT(uint32_t, script, 0x10) - begin;
        eventCount = static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 3);
    }

    for (uint32_t index = 0; index < eventCount; ++index) {
        script = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
        const uint32_t entries = CSF_AT(uint32_t, script, 0x0C);
        void* entry = reinterpret_cast<void*>(entries + 8 * index);
        const uint32_t eventKey = CSF_AT(uint32_t, entry, 0);
        const uint32_t recordType = record[1];

        if (eventKey & 0x8000000) {
            if (recordType != 1 || record[9] != (eventKey & 0xF7FFFFFF))
                continue;
        } else if (eventKey != recordType) {
            continue;
        }

        uint32_t result = static_cast<uint32_t>(CopyStateFromRecord(
            self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(record))));
        int16_t* action = reinterpret_cast<int16_t*>(
            CSF_AT(uint32_t, entry, 0x04));

        if (action) {
            GetGlobalBlock8AFDA0();
            C_008313f8* definition = reinterpret_cast<C_008313f8*>(
                GetDwordField4(static_cast<int>(reinterpret_cast<uintptr_t>(action))));
            S_00467cc0_ExecuteAction(
                reinterpret_cast<S_00467cc0*>(self), nullptr, action, definition);

            result = definition->field_0004;
            if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x01))
                continue;
        }

        if (index != UINT32_MAX)
            return S_00467cc0_StartActions(self);
        return static_cast<char>(result);
    }

    return static_cast<char>(eventCount);
}

HOOK(0x00468310, S_0045bda0_HandleEventRecord, "auto");
