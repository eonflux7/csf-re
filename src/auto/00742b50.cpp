#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SharedDispatch_65E950, 0x0065E950, (void* self, void* edx, int a1, int a2, int a3));
CSF_FUNC(char, __fastcall, NetCogible_HandleMsg740080, 0x00740080, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogible_HandleMsg7400F0, 0x007400F0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, NetCogible_HandleMsg7423D0, 0x007423D0, (void* self, void* edx, void* arg0));
CSF_FUNC(bool, __fastcall, NetCogible_HandleMsg742540, 0x00742540, (void* self, void* edx, void* arg0));
CSF_FUNC(char, __fastcall, NetCogible_HandleMsg7426D0, 0x007426D0, (void* self, void* edx, void* arg0));
CSF_FUNC(void, __fastcall, NetCogible_HandleMsg7427E0, 0x007427E0, (void* self, void* edx, void* arg0));
CSF_FUNC(void, __fastcall, NetCogible_HandleMsg7428E0, 0x007428E0, (void* self, void* edx, void* arg0));

static char __fastcall NetCogible_DispatchMessage(NetCogible* self, void* /*edx*/, void* stats, int msgId, void* data)
{
    if ((uint16_t)msgId == self->field_0020) {
        NetCogible_HandleMsg7400F0(stats, nullptr);
        return 1;
    }
    if ((uint16_t)msgId == self->field_0022) {
        NetCogible_HandleMsg7423D0(stats, nullptr, data);
        return 1;
    }
    if ((uint16_t)msgId == self->field_0024) {
        NetCogible_HandleMsg742540(stats, nullptr, data);
        return 1;
    }
    if ((uint16_t)msgId == self->field_0026) {
        NetCogible_HandleMsg7426D0(stats, nullptr, data);
        return 1;
    }
    if ((uint16_t)msgId == self->field_0028) {
        NetCogible_HandleMsg7427E0(stats, nullptr, data);
        return 1;
    }
    if ((uint16_t)msgId == self->field_002a) {
        NetCogible_HandleMsg7428E0(stats, nullptr, data);
        return 1;
    }
    if ((uint16_t)msgId == self->field_002c) {
        NetCogible_HandleMsg740080(stats, nullptr);
        return 1;
    }
    return (char)SharedDispatch_65E950(self, nullptr, (int)stats, msgId, (int)data);
}

HOOK(0x00742B50, NetCogible_DispatchMessage, "auto");
