#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, CGuiEdit_DeleteCharacter, 0x00721C10,
         (CGuiEdit* self, void* edx));
CSF_FUNC(int, __fastcall, DeleteAtTextPosition, 0x007223E0,
         (CGuiEdit* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiEdit_ResetScrollIfTextPresent, 0x007224A0,
         (CGuiEdit* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiEdit_HandleBackspace, 0x007224C0,
         (CGuiEdit* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiEdit_MoveTextRangeLeft, 0x00722560,
         (CGuiEdit* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiEdit_MoveTextRangeRight, 0x007225B0,
         (CGuiEdit* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiEdit_MoveIndicesLeft, 0x00722600,
         (CGuiEdit* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiEdit_MoveIndicesRight, 0x007226E0,
         (CGuiEdit* self, void* edx));

using NavigationDispatchTable = std::uint32_t[13];
CSF_GLOBAL(NavigationDispatchTable, g_NavigationDispatchTable, 0x0072284C);

// The original jump table is represented below by its decoded key cases.
// EBX remains an implicit live-in to the original navigation callees.
static __declspec(noinline) __attribute__((optnone))
unsigned char __fastcall CGuiEdit_HandleNavigationKey(
    CGuiEdit* self, void* /*edx*/, int key)
{
    unsigned char handled = 0;

    if (self->field_01c7 != 0)
    {
        switch (key)
        {
        case 199:
            CGuiEdit_ResetScrollIfTextPresent(self, nullptr);
            handled = 1;
            break;
        case 201:
            CGuiEdit_MoveTextRangeLeft(self, nullptr);
            handled = 1;
            break;
        case 203:
            CGuiEdit_MoveIndicesLeft(self, nullptr);
            handled = 1;
            break;
        case 205:
            CGuiEdit_MoveIndicesRight(self, nullptr);
            handled = 1;
            break;
        case 207:
            CGuiEdit_HandleBackspace(self, nullptr);
            handled = 1;
            break;
        case 209:
            CGuiEdit_MoveTextRangeRight(self, nullptr);
            handled = 1;
            break;
        case 211:
            DeleteAtTextPosition(self, nullptr);
            handled = 1;
            break;
        default:
            break;
        }
    }

    if (key == 14)
    {
        CGuiEdit_DeleteCharacter(self, nullptr);
        handled = 1;
    }

    return handled;
}

HOOK(0x007227D0, CGuiEdit_HandleNavigationKey, "auto");
