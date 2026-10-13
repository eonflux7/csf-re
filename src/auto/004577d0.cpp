#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_FUNC(std::uint64_t, __stdcall, ComputeControlAnimationKey, 0x00444BB0, (int controlAddress, int direction, std::uint32_t* flags, char includeEntries));
CSF_FUNC(int, __fastcall, HudPortraits_ApplyNameToControl, 0x00457400, (void* self, void* edx, char* a2, int a5, int a6));

// __thiscall(this, char* name, int control) ret 8
static int __fastcall ApplyStringToObject(void* self, void* /*edx*/, char* a2, int a3)
{
    // Field at +0x7C of the control object: forwarded as the second argument.
    int controlSubId = CSF_AT(int, reinterpret_cast<void*>(a3), 0x7C);

    std::uint32_t animFlags = 0;

    CGuiControl* guiControl = reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568());
    CGuiControl_GetColorAnim(guiControl, nullptr);

    int animKey = static_cast<int>(ComputeControlAnimationKey(a3, 0, &animFlags, 1));

    return HudPortraits_ApplyNameToControl(self, nullptr, a2, controlSubId, animKey);
}

HOOK(0x4577D0, ApplyStringToObject, "auto");
