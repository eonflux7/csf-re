#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x72C580: CGuiPanel constructor (_DWORD *__thiscall(this, int a2, int a3), ret 8)
CSF_FUNC(void*, __fastcall, CGuiPanel_Construct, 0x72C580, (void* self, void* /*edx*/, int a2, int a3));

// CGuiRadioGroup constructor: runs the CGuiPanel base constructor, then installs the
// CGuiRadioGroup vtable (0x008660C8) and returns this.
static CGuiRadioGroup* __fastcall CGuiRadioGroup_Construct(CGuiRadioGroup* self, void* /*edx*/, int a2, int a3)
{
    CGuiPanel_Construct(self, nullptr, a2, a3);
    *reinterpret_cast<uint32_t*>(self) = CSF_ADDR_VT(0x008660C8);  // CGuiRadioGroup vtable
    return self;
}

HOOK(0x72E790, CGuiRadioGroup_Construct, "auto");
