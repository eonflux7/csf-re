#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CCharActionFsm_EnterBaseState, 0x00480570, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState12, 0x004836B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState13, 0x004836D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState15, 0x004837F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState16, 0x00483810, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState17, 0x00483860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState22, 0x004838E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState23, 0x00483900, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState24, 0x00483990, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState25, 0x004839D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState26, 0x00483A20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState38, 0x00483C30, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CCharActionFsm_TryEnterActionState1, 0x00484590, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CCharActionFsm_TryEnterActionState4, 0x004845D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState14, 0x00484A10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CCharActionFsm_EnterActionState34, 0x00484A40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState18, 0x00485190, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState11, 0x00485200, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState31, 0x004852B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState32, 0x00485360, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState36, 0x00485410, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState10, 0x00486180, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState37, 0x00486300, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState21, 0x004877D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_EnterActionState28, 0x00487890, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_EnterLinkedActionState40, 0x0048D610, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_EnterLinkedActionState41, 0x0048D6C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_EnterLinkedActionState39, 0x0048D8D0, (void* self, void* edx));

static char __fastcall CCharActionFsm_EnterState(
    CCharActionFsm* self, void* /*edx*/, int state)
{
    self->field_0010 = state;
    self->field_0069 = 1;
    self->field_006c = 0.0f;

    switch (state)
    {
    case 1:
    {
        const char result = CCharActionFsm_TryEnterActionState1(self, nullptr);
        if (result)
            return result;
        return static_cast<char>(CCharActionFsm_EnterBaseState(self, nullptr, state));
    }
    case 4:
    {
        const char result = CCharActionFsm_TryEnterActionState4(self, nullptr);
        if (result)
            return result;
        return static_cast<char>(CCharActionFsm_EnterBaseState(self, nullptr, state));
    }
    case 10:
        return static_cast<char>(CCharActionFsm_EnterActionState10(self, nullptr));
    case 11:
        return static_cast<char>(CCharActionFsm_EnterActionState11(self, nullptr));
    case 12:
        return static_cast<char>(CCharActionFsm_EnterActionState12(self, nullptr));
    case 13:
        return static_cast<char>(CCharActionFsm_EnterActionState13(self, nullptr));
    case 14:
        return static_cast<char>(CCharActionFsm_EnterActionState14(self, nullptr));
    case 15:
        return static_cast<char>(CCharActionFsm_EnterActionState15(self, nullptr));
    case 16:
        return static_cast<char>(CCharActionFsm_EnterActionState16(self, nullptr));
    case 17:
        return static_cast<char>(CCharActionFsm_EnterActionState17(self, nullptr));
    case 18:
        return static_cast<char>(CCharActionFsm_EnterActionState18(self, nullptr));
    case 19:
        return static_cast<char>(self->vUpdateWeaponSelection(8));
    case 20:
    case 30:
    case 33:
        return static_cast<char>(self->vUpdateWeaponSelection(12));
    case 21:
        return static_cast<char>(CCharActionFsm_EnterActionState21(self, nullptr));
    case 22:
        return static_cast<char>(CCharActionFsm_EnterActionState22(self, nullptr));
    case 23:
        return static_cast<char>(CCharActionFsm_EnterActionState23(self, nullptr));
    case 24:
        return static_cast<char>(CCharActionFsm_EnterActionState24(self, nullptr));
    case 25:
        return static_cast<char>(CCharActionFsm_EnterActionState25(self, nullptr));
    case 26:
        return static_cast<char>(CCharActionFsm_EnterActionState26(self, nullptr));
    case 27:
        return static_cast<char>(self->vUpdateWeaponSelection(16));
    case 28:
        return static_cast<char>(CCharActionFsm_EnterActionState28(self, nullptr));
    case 29:
        return static_cast<char>(state - 1);
    case 31:
        return static_cast<char>(CCharActionFsm_EnterActionState31(self, nullptr));
    case 32:
        return static_cast<char>(CCharActionFsm_EnterActionState32(self, nullptr));
    case 34:
        return CCharActionFsm_EnterActionState34(self, nullptr);
    case 35:
        return static_cast<char>(self->vUpdateWeaponSelection(13));
    case 36:
        return static_cast<char>(CCharActionFsm_EnterActionState36(self, nullptr));
    case 37:
        return static_cast<char>(CCharActionFsm_EnterActionState37(self, nullptr));
    case 38:
        return static_cast<char>(CCharActionFsm_EnterActionState38(self, nullptr));
    case 39:
        return static_cast<char>(CharActionFsm_EnterLinkedActionState39(
            reinterpret_cast<void*>(self->field_00d4), nullptr));
    case 40:
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(
            CharActionFsm_EnterLinkedActionState40(
                reinterpret_cast<void*>(self->field_00d4), nullptr)));
    case 41:
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(
            CharActionFsm_EnterLinkedActionState41(
                reinterpret_cast<void*>(self->field_00d4), nullptr)));
    default:
        return static_cast<char>(CCharActionFsm_EnterBaseState(self, nullptr, state));
    }
}

HOOK(0x00488590, CCharActionFsm_EnterState, "auto");
