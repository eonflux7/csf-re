#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiEdit_ResetScrollIfTextPresent, 0x007224A0, (CGuiEdit* self, void*));
CSF_FUNC(int, __fastcall, CGuiEdit_HandleBackspace, 0x007224C0, (CGuiEdit* self, void*));
CSF_FUNC(int, __fastcall, CGuiEdit_MoveTextRangeLeft, 0x00722560, (CGuiEdit* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiEdit_MoveTextRangeRight, 0x007225B0, (CGuiEdit* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiEdit_MoveIndicesLeft, 0x00722600, (CGuiEdit* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiEdit_MoveIndicesRight, 0x007226E0, (CGuiEdit* self, void*));

static char __fastcall CGuiEdit_HandleEditKey(CGuiEdit* self, void* /*edx*/, int key)
{
    if (!self->vIsSelected())
        return 0;

    if (!self->field_01c7)
        return 0;

    switch (key)
    {
    case 0:
        CGuiEdit_MoveTextRangeLeft(self, nullptr);
        break;
    case 1:
        CGuiEdit_MoveTextRangeRight(self, nullptr);
        break;
    case 2:
        CGuiEdit_MoveIndicesLeft(self, nullptr);
        break;
    case 3:
        CGuiEdit_MoveIndicesRight(self, nullptr);
        break;
    case 9:
        CGuiEdit_ResetScrollIfTextPresent(self, nullptr);
        break;
    case 11:
        CGuiEdit_HandleBackspace(self, nullptr);
        break;
    default:
        return 0;
    }

    return 1;
}

HOOK(0x00722880, CGuiEdit_HandleEditKey, "auto");
