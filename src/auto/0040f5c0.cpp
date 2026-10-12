#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, AllocateSubobjectHandle, 0x0040A430, (int type));
CSF_FUNC(char, __fastcall, C_0082ddf8_RefreshFlags, 0x0040F150, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0082e520_PrepareInputForBinding, 0x0041E050, (C_0082e520* self, void* , int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));

static char __fastcall C_0082ddf8_InitializeBinding(
    C_0082ddf8* self, void* /*edx*/, int context, uint32_t* input)
{
    self->vSlot24();
    C_0082e520_PrepareInputForBinding(
        reinterpret_cast<C_0082e520*>(self), nullptr,
        context, reinterpret_cast<int>(input));

    uint32_t* subobject = AllocateSubobjectHandle(10);
    self->field_001c = reinterpret_cast<uint32_t>(subobject);

    const char initialized =
        (reinterpret_cast<char(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subobject, 0)[0x8 / 4]))(
                subobject, nullptr, context);

    if (initialized)
    {
        self->field_0010 = 0;
        if (CSF_AT(uint32_t, input, 0x1C))
        {
            void* accessor = reinterpret_cast<void*>(
                GlobalInt_ResolveAccessor(
                    reinterpret_cast<void*>(GetGlobalInt()), nullptr));

            (reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, accessor, 0)[0x10 / 4]))(
                    accessor, nullptr);

            uint8_t* name;
            if (CSF_AT(uint32_t, input, 0x20) < 0x10u)
                name = reinterpret_cast<uint8_t*>(input) + 0xC;
            else
                name = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, input, 0xC));

            self->field_0010 = GlobalInt_FindNameId(name);
        }

        const bool wantsFlag1 = (CSF_AT(uint32_t, input, 0x4) & 1) == 1;
        if (((self->field_0014 & 1) == 1) != wantsFlag1)
        {
            self->field_0014 = wantsFlag1
                ? self->field_0014 | 1
                : self->field_0014 & 0xFFFFFFFE;
            C_0082ddf8_RefreshFlags(self, nullptr);
        }

        const bool wantsFlag2 = (CSF_AT(uint32_t, input, 0x4) & 2) == 2;
        if (((self->field_0014 & 2) == 2) != wantsFlag2)
        {
            self->field_0014 = wantsFlag2
                ? self->field_0014 | 2
                : self->field_0014 & 0xFFFFFFFD;
            C_0082ddf8_RefreshFlags(self, nullptr);
        }

        const bool wantsFlag4 = (CSF_AT(uint32_t, input, 0x4) & 4) == 4;
        if (((self->field_0014 & 4) == 4) != wantsFlag4)
        {
            self->field_0014 = wantsFlag4
                ? self->field_0014 | 4
                : self->field_0014 & 0xFFFFFFFB;
            C_0082ddf8_RefreshFlags(self, nullptr);
        }
    }

    return initialized;
}

HOOK(0x0040F5C0, C_0082ddf8_InitializeBinding, "auto");
