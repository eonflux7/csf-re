#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x506AC0, ());
CSF_FUNC(void*, __fastcall, CMenuEula_LoadTextIntoNameAndSetState, 0x533660, (CMenuEula* self, void* edx));
CSF_FUNC(void*, __fastcall, CMenuEula_RefreshState, 0x532E40, (CMenuEula* self, void* edx));
CSF_FUNC(void*, __fastcall, CMenuEula_UpdateNavigationButtons_532ED0, 0x532ED0, (CMenuEula* self, void* edx));
CSF_FUNC(void*, __fastcall, CMenuEula_UpdateNavigationButtons_532F70, 0x532F70, (CMenuEula* self, void* edx));

static void* __fastcall CMenuEula_RunStateMachine(CMenuEula* self, void* /*edx*/, int /*unusedArg*/)
{
    Nullsub_506AC0();

    const uint32_t state = self->field_0018;
    void* result = reinterpret_cast<void*>(static_cast<uintptr_t>(state));

    switch (state)
    {
    case 0u:
    {
        void* controlObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_002c));
        result = reinterpret_cast<void*(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, controlObject, 0)[26])(controlObject, nullptr, 0, 1, 1);
        self->field_0018 = 1;
        break;
    }
    case 1u:
        result = CMenuEula_LoadTextIntoNameAndSetState(self, nullptr);
        break;
    case 2u:
        result = CMenuEula_RefreshState(self, nullptr);
        break;
    case 3u:
        self->field_0018 = 4;
        break;
    case 4u:
        result = CMenuEula_UpdateNavigationButtons_532ED0(self, nullptr);
        break;
    case 5u:
        self->field_0018 = 6;
        break;
    case 6u:
        result = CMenuEula_UpdateNavigationButtons_532F70(self, nullptr);
        break;
    default:
        break;
    }

    return result;
}

HOOK(0x5336C0, CMenuEula_RunStateMachine, "auto");
