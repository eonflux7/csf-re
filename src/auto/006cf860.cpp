#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __fastcall, C_00860c88_Clear, 0x006CEA20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxCollisionShapes_ClearAuxiliaryShape, 0x006CFBD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00861068_Clear, 0x006D5300, (void* self, void* edx));

static void __fastcall C_00860c98_Destroy(C_00860c98* self, void* /*edx*/)
{
    void* shape = reinterpret_cast<void*>(self->field_0010);
    self->vtbl = CSF_VTBL(0x00860CBC);
    C_00860c88_Clear(shape, nullptr);
    C_00861068_Clear(reinterpret_cast<void*>(self->field_000c), nullptr);

    void* object = reinterpret_cast<void*>(self->field_0008);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[0x1C / 4])(
        object, nullptr);

    object = reinterpret_cast<void*>(self->field_0004);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[0x20 / 4])(
        object, nullptr);

    GfxCollisionShapes_ClearAuxiliaryShape(
        reinterpret_cast<void*>(self->field_0014), nullptr);

    object = reinterpret_cast<void*>(self->field_0014);
    self->field_0068 = 0;
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[4 / 4])(
            object, nullptr, 1);
        self->field_0014 = 0;
    }

    auto* sphere = reinterpret_cast<C_00860c88*>(self->field_0010);
    if (sphere)
    {
        sphere->vSlot4(1);
        self->field_0010 = 0;
    }

    auto* ellipsoid = reinterpret_cast<C_00861068*>(self->field_000c);
    if (ellipsoid)
    {
        ellipsoid->vSlot4(1);
        self->field_000c = 0;
    }

    object = reinterpret_cast<void*>(self->field_0008);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[4 / 4])(
            object, nullptr, 1);
        self->field_0008 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0004);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[4 / 4])(
            object, nullptr, 1);
        self->field_0004 = 0;
    }

    if (self->field_006c)
    {
        RwFrameDestroy(static_cast<int>(self->field_006c));
        self->field_006c = 0;
    }

    self->vtbl = CSF_VTBL(0x00860C98);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006CF860, C_00860c98_Destroy, "auto");
