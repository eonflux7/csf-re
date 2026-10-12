#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_537A50, 0x00537A50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_536E90, 0x00536E90, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_5383C0, 0x005383C0, (void* self, void* edx, int a3));
CSF_FUNC(void, __fastcall, sub_538240, 0x00538240, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, sub_5360B0, 0x005360B0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_5369A0, 0x005369A0, (void* self, void* edx));

// Dispatches the instancing scene/result request to the handler for the current mode
// stored at this+0x130. Mode 4 and any out-of-range value do nothing.
static void __fastcall SceneInstancing_DispatchByMode(S_00537cd0* self, void* /*edx*/, S_0054ecf0* a2)
{
    sub_537A50(self, nullptr);

    switch (CSF_AT(int, self, 0x130))
    {
    case 0:
        sub_536E90(self, nullptr, reinterpret_cast<int>(a2));
        break;
    case 1:
        sub_5383C0(self, nullptr, reinterpret_cast<int>(a2));
        break;
    case 2:
        sub_538240(self, nullptr, a2);
        break;
    case 3:
        sub_5360B0(self, nullptr, reinterpret_cast<int>(a2));
        break;
    case 5:
        sub_5369A0(self, nullptr);
        break;
    default:
        return;
    }
}

HOOK(0x00538430, SceneInstancing_DispatchByMode, "auto");
