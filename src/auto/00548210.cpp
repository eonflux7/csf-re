#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004f1f00_UpdateSubtitlePanel, 0x004F3370, (S_004f1f00* self, void* , float elapsedTime));
CSF_FUNC(void, __fastcall, HudStringShow_ShowDefault, 0x004F3EF0, (void* self, void* , void* text, float arg1, float arg2));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, S_00522010_AccumulateAndPropagateTime, 0x005231D0, (S_00522010* self, void* , int deltaBits));
CSF_FUNC(int, __fastcall, CMenuExtras_ApplyPanelMode, 0x00546590, (void* self, void* , char flag));
CSF_FUNC(int, __fastcall, CMenuExtras_ResetActiveExtra, 0x005477E0, (CMenuExtras* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuExtras_HandleBackAction, 0x00547D50, (CMenuExtras* self, void* , int));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(double, __fastcall, ForwardMetricOrMissingValue, 0x006B15B0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, IsGlobalIntGateOpen, 0x006B1680, (void* self, void* edx));

static char __fastcall CMenuExtras_Update(CMenuExtras* self, void* /*edx*/, float elapsedTime)
{
    if (self->field_0046)
        return static_cast<char>(CMenuExtras_HandleBackAction(self, nullptr, 1));

    std::memcpy(&self->field_0024, &elapsedTime, sizeof(elapsedTime));
    int deltaBits;
    std::memcpy(&deltaBits, &elapsedTime, sizeof(deltaBits));

    auto* timeState = reinterpret_cast<S_00522010*>(GetGlobalValue_521BB0());
    S_00522010_AccumulateAndPropagateTime(timeState, nullptr, deltaBits);
    SceneLoader_NoOp();

    if (self->field_0047)
    {
        const uint32_t begin = self->field_0050;
        const uint32_t count = begin
            ? static_cast<uint32_t>(static_cast<int32_t>(self->field_0054 - begin) >> 5)
            : 0;
        const uint32_t index = self->field_005c;
        if (count > index)
        {
            const auto* cueTime =
                reinterpret_cast<const float*>(self->field_0050 + 32 * index);
            void* playback = reinterpret_cast<void*>(GetGlobalInt());
            if (ForwardMetricOrMissingValue(playback, nullptr) > *cueTime)
            {
                const uint32_t currentIndex = self->field_005c;
                self->field_005c = currentIndex + 1;
                void* text = reinterpret_cast<void*>(self->field_0050 + 32 * currentIndex + 4);

                // The original passes the integer bit pattern -1 as a float argument.
                const int sentinelBits = -1;
                float sentinel;
                std::memcpy(&sentinel, &sentinelBits, sizeof(sentinel));
                HudStringShow_ShowDefault(
                    reinterpret_cast<void*>(self->field_0048), nullptr,
                    text, 4.5f, sentinel);
            }
        }

        S_004f1f00_UpdateSubtitlePanel(
            reinterpret_cast<S_004f1f00*>(self->field_0048), nullptr, elapsedTime);
        void* playback = reinterpret_cast<void*>(GetGlobalInt());
        if (IsGlobalIntGateOpen(playback, nullptr))
            CMenuExtras_ResetActiveExtra(self, nullptr);
    }

    char result = static_cast<char>(self->field_0044);
    if (result)
    {
        result = static_cast<char>(CMenuExtras_ApplyPanelMode(
            self, nullptr, static_cast<char>(self->field_0045)));
        self->field_0044 = 0;
    }
    return result;
}

HOOK(0x00548210, CMenuExtras_Update, "auto");
