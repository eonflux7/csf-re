#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Slot byte offsets: 0x16C -> vtbl[0x5B] (no args), 0xFC -> vtbl[0x3F] (one const char* arg).
static constexpr std::size_t kVtblSlotResetBindings = 0x16C / 4;
static constexpr std::size_t kVtblSlotFindButton = 0xFC / 4;
static constexpr uintptr_t kDMatchNameStepBytes = 30;

CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x718660, (void* self, void* edx));
CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);

static int __fastcall CMenuMpGameMode_InitDMatchButtonSlots(CMenuMpGameMode* self, void* /*edx*/, int a2, int /*a3*/)
{
    self->field_0014 = static_cast<uint32_t>(a2);
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    void* menuObject = reinterpret_cast<void*>(self->field_0018);
    void** menuVtbl = CSF_AT(void**, menuObject, 0);
    ((void(__fastcall*)(void*, void*))menuVtbl[kVtblSlotResetBindings])(menuObject, nullptr);

    int foundCount = 0;
    const char* nameCursor = CSF_STR(0x0089A570, "BotonDMatch");
    uint32_t* slot = &self->field_0020;

    while (static_cast<int32_t>(reinterpret_cast<intptr_t>(nameCursor)) <
           static_cast<int32_t>(reinterpret_cast<intptr_t>(&g_SelectedAccountIndex)))
    {
        void* obj = reinterpret_cast<void*>(self->field_0018);
        void** objVtbl = CSF_AT(void**, obj, 0);
        int found = ((int(__fastcall*)(void*, void*, const char*))objVtbl[kVtblSlotFindButton])(obj, nullptr, nameCursor);
        *slot = static_cast<uint32_t>(found);
        if (found)
        {
            if (foundCount == 0)
                CGuiSlider_ForwardSelfToParent(reinterpret_cast<void*>(found), nullptr);
            ++foundCount;
        }
        nameCursor += kDMatchNameStepBytes;
        ++slot;
    }

    return static_cast<int>(self->field_0018);
}

HOOK(0x00565D40, CMenuMpGameMode_InitDMatchButtonSlots, "auto");
