#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// The pointer type matches the hook for musttail; the object is opaque.
static __declspec(naked) int __fastcall TailForwardToLinkedObject(
    CGuiStatic* /*object*/, void* /*edx*/)
{
    __asm {
        mov eax, ecx
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 7Ch]
    }
}

static int __fastcall CGuiStatic_QueryOrForwardToLinkedObject(
    CGuiStatic* self, void* /*edx*/)
{
    const int result =
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xAC / 4])(
            self, nullptr);

    if (static_cast<uint8_t>(result) != 0)
        return result;

    const uint32_t linkedObject = self->field_0138;
    if (linkedObject == 0)
        return 0;

    [[clang::musttail]] return TailForwardToLinkedObject(
        reinterpret_cast<CGuiStatic*>(linkedObject), nullptr);
}

HOOK(0x720B70, CGuiStatic_QueryOrForwardToLinkedObject, "auto");
