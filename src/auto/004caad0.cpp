#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction0, 0x004C5230, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction1, 0x004C55D0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction2, 0x004BFDD0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction3, 0x004C9880, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction4, 0x004BFE20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction5, 0x004BC960, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction6, 0x004BC990, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction7, 0x004BC930, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction8, 0x004BFD70, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction9, 0x004BC9C0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction10, 0x004BC9F0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction11, 0x004BCA20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction12, 0x004BFE70, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction13, 0x004BCA50, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction14, 0x004BCA80, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction15, 0x004BCAB0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction16, 0x004BCAE0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction17, 0x004BCB10, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction18, 0x004BFF20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction19, 0x004BFF80, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction20, 0x004BFFE0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction21, 0x004C0040, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction22, 0x004BFED0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction23, 0x004C00A0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction24, 0x004C0100, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction25, 0x004C0160, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction26, 0x004BCB40, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction27, 0x004C01C0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction28, 0x004C0220, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction29, 0x004C0280, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction30, 0x004C02E0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction31, 0x004BCB70, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction32, 0x004C0340, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004bc930_ExecuteAction33, 0x004C03A0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static bool __fastcall S_004bc930_DispatchNextQueuedAction(
    S_004bc930* self, void* /*edx*/, int argument)
{
    const std::uint32_t remainingCount = self->field_00d4 - 1;
    auto* actions = reinterpret_cast<int*>(self->field_00cc);
    const int action = actions[0];

    if (remainingCount != 0)
    {
        self->field_00d4 = remainingCount;
        _memcpy(actions, actions + 1, 4 * remainingCount);
    }
    else
    {
        self->field_00d4 = 0;
    }

    switch (action)
    {
    case 0:  return S_004bc930_ExecuteAction0(self, nullptr, argument) == 0;
    case 1:  return S_004bc930_ExecuteAction1(self, nullptr, argument) == 0;
    case 2:  return S_004bc930_ExecuteAction2(self, nullptr, argument) == 0;
    case 3:  return S_004bc930_ExecuteAction3(self, nullptr, argument) == 0;
    case 4:  return S_004bc930_ExecuteAction4(self, nullptr, argument) == 0;
    case 5:  return S_004bc930_ExecuteAction5(self, nullptr, argument) == 0;
    case 6:  return S_004bc930_ExecuteAction6(self, nullptr, argument) == 0;
    case 7:  return S_004bc930_ExecuteAction7(self, nullptr, argument) == 0;
    case 8:  return S_004bc930_ExecuteAction8(self, nullptr, argument) == 0;
    case 9:  return S_004bc930_ExecuteAction9(self, nullptr, argument) == 0;
    case 10: return S_004bc930_ExecuteAction10(self, nullptr, argument) == 0;
    case 11: return S_004bc930_ExecuteAction11(self, nullptr, argument) == 0;
    case 12: return S_004bc930_ExecuteAction12(self, nullptr, argument) == 0;
    case 13: return S_004bc930_ExecuteAction13(self, nullptr, argument) == 0;
    case 14: return S_004bc930_ExecuteAction14(self, nullptr, argument) == 0;
    case 15: return S_004bc930_ExecuteAction15(self, nullptr, argument) == 0;
    case 16: return S_004bc930_ExecuteAction16(self, nullptr, argument) == 0;
    case 17: return S_004bc930_ExecuteAction17(self, nullptr, argument) == 0;
    case 18: return S_004bc930_ExecuteAction18(self, nullptr, argument) == 0;
    case 19: return S_004bc930_ExecuteAction19(self, nullptr, argument) == 0;
    case 20: return S_004bc930_ExecuteAction20(self, nullptr, argument) == 0;
    case 21: return S_004bc930_ExecuteAction21(self, nullptr, argument) == 0;
    case 22: return S_004bc930_ExecuteAction22(self, nullptr, argument) == 0;
    case 23: return S_004bc930_ExecuteAction23(self, nullptr, argument) == 0;
    case 24: return S_004bc930_ExecuteAction24(self, nullptr, argument) == 0;
    case 25: return S_004bc930_ExecuteAction25(self, nullptr, argument) == 0;
    case 26: return S_004bc930_ExecuteAction26(self, nullptr, argument) == 0;
    case 27: return S_004bc930_ExecuteAction27(self, nullptr, argument) == 0;
    case 28: return S_004bc930_ExecuteAction28(self, nullptr, argument) == 0;
    case 29: return S_004bc930_ExecuteAction29(self, nullptr, argument) == 0;
    case 30: return S_004bc930_ExecuteAction30(self, nullptr, argument) == 0;
    case 31: return S_004bc930_ExecuteAction31(self, nullptr, argument) == 0;
    case 32: return S_004bc930_ExecuteAction32(self, nullptr, argument) == 0;
    case 33: return S_004bc930_ExecuteAction33(self, nullptr, argument) == 0;
    default: return true;
    }
}

HOOK(0x004CAAD0, S_004bc930_DispatchNextQueuedAction, "auto");
