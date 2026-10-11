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

CSF_FUNC(int, __fastcall, sub_4092A0, 0x004092A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_409300, 0x00409300, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_409930, 0x00409930, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Slot 0x28 of a3 (typed wrapper vGetField016c in the decompiler output): returns a sub-object pointer.
static void* __fastcall C_00833908_GetField016c(C_00833908* self)
{
    return ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);
}

// Object returned by the field getter: slot 0x18 returns a further object, slot 0x28 is called on it.
static void* __fastcall ObjectRet_Slot18(void* self)
{
    return ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x18 / 4])(self, nullptr);
}

static void __fastcall ObjectVoid_Slot28(void* self)
{
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);
}

static char __fastcall C_0082d844_InitializeFromSource(C_0082d844* self, void* /*edx*/, C_00833908* source)
{
    // NOTE: the original reads st0 (caller's float input) as the initial value when the
    // getter chain returns a non-null object; that input is taken as 0.0 here.
    float initialValue = 0.0f;

    self->field_0008 = reinterpret_cast<uint32_t>(source);
    void* fieldObject = C_00833908_GetField016c(source);
    if (fieldObject)
    {
        void* innerObject = ObjectRet_Slot18(fieldObject);
        ObjectVoid_Slot28(innerObject);
    }
    else
    {
        initialValue = 0.0f;
    }

    self->field_000c = initialValue;
    self->field_0010 = 0.0f;
    self->field_0014 = 0.0f;
    self->field_0018 = 80.0f;
    self->field_001c = 0;

    float resultValue;
    if (sub_4092A0(self, nullptr) == 1)
        resultValue = 0.0f;
    else
        resultValue = self->field_0020 * g_fZero;

    self->field_0010 = resultValue;
    sub_409300(self, nullptr);
    sub_409930(self, nullptr);
    self->field_0024 = 0;
    return 1;
}

HOOK(0x409B40, C_0082d844_InitializeFromSource, "auto");
