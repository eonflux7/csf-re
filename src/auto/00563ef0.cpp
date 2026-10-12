#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpSessions_DispatchThunk, 0x00560480, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CMenuMpSessions_RefreshOptionControls, 0x00560960, (CMenuMpSessions* self, void*));
CSF_FUNC(int, __fastcall, MenuMpSessions_HandleState1Or3, 0x00560BD0, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpSessions_SetControlsActive, 0x00560F80, (CMenuMpSessions* self, void*));
CSF_FUNC(char, __fastcall, CMenuMpSessions_UpdateAutomaticSessionSelection, 0x005637C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpSessions_CreateAutomaticSession, 0x00563930, (void* self, void* edx));

static char __fastcall CMenuMpSessions_UpdateSessionState(
    CMenuMpSessions* self, void* /*edx*/)
{
    bool refreshControls = false;
    if (self->field_01f0)
    {
        self->field_01b8 = 0;
        self->field_01bc = 0;
        self->field_001c = 0;
        self->field_01c0 = 2;
        refreshControls = true;
        self->field_01f0 = 0;
    }

    const uint32_t phase = self->field_01e4;
    char result = static_cast<char>(phase - 1);
    switch (phase)
    {
        case 1u:
            result = CMenuMpSessions_UpdateAutomaticSessionSelection(self, nullptr);
            break;
        case 2u:
            result = static_cast<char>(
                CMenuMpSessions_CreateAutomaticSession(self, nullptr));
            break;
        case 3u:
            result = static_cast<char>(self->field_01e9);
            if (result)
                self->field_01f1 = 1;
            break;
        case 4u:
            result = static_cast<char>(
                CMenuMpSessions_DispatchThunk(self, nullptr));
            break;
        default:
            break;
    }

    if (refreshControls)
    {
        const uint32_t state = self->field_01e0;
        switch (state)
        {
            case 0u:
                CMenuMpSessions_RefreshOptionControls(self, nullptr);
                // The void callee provides no defined return value.
                return 0;
            case 1u:
            case 3u:
                return static_cast<char>(
                    MenuMpSessions_HandleState1Or3(self, nullptr));
            case 2u:
            case 4u:
                return static_cast<char>(
                    CMenuMpSessions_SetControlsActive(self, nullptr));
            default:
                return static_cast<char>(state);
        }
    }

    return result;
}

HOOK(0x00563EF0, CMenuMpSessions_UpdateSessionState, "auto");
