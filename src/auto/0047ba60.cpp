#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));

// Original: TCharTask::sub_47BA60, __thiscall, ret 8.
// If the current item of the owner's controller has a nonzero count, look up the
// task at (this->field_000c + 40); otherwise signal rotation complete with 0.
static void __fastcall TCharTask_ContinueOrCompleteRotation(void* self, void* /*edx*/, int a2, int a3)
{
    (void)a2;
    (void)a3;

    void* owner = CSF_AT(void*, self, 0x08);
    void* context = CSF_AT(void*, owner, 0x14);
    void* controller = CSF_AT(void*, context, 0x44);

    // Virtual slot 0xD8 / 4 = 54, no arguments.
    void* currentItem = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, controller, 0)[0xD8 / 4])(controller, nullptr);

    if (GetCurrentItemCount(currentItem, nullptr) != 0)
    {
        int taskOffset = CSF_AT(int, self, 0x0C) + 0x28;
        CharTask_LookupByOffset(self, nullptr, taskOffset);
    }
    else
    {
        TFncGirar_OnRotationComplete(self, nullptr, 0);
    }
}

HOOK(0x0047BA60, TCharTask_ContinueOrCompleteRotation, "auto");
