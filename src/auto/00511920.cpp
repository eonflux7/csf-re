#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuManager_ReleaseOwnedScreens, 0x0050D4C0, (CMenuManager* self, void*));

static int __fastcall C_008407a8_ReleaseOwnedScreens(C_008407a8* self, void* /*edx*/)
{
    int result = CMenuManager_ReleaseOwnedScreens(self, nullptr);

    auto* screen = reinterpret_cast<C_008407a8*>(self->field_0134);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0134 = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_0118);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0118 = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_011c);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_011c = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_0114);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0114 = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_0110);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0110 = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_0124);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0124 = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_0120);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0120 = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_0128);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0128 = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_012c);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_012c = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_0130);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0130 = 0;
    }

    screen = reinterpret_cast<C_008407a8*>(self->field_0138);
    if (screen)
    {
        result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
        self->field_0138 = 0;
    }

    if (self->field_0140)
    {
        screen = reinterpret_cast<C_008407a8*>(self->field_0064);
        if (screen)
        {
            result = reinterpret_cast<int>(screen->vScalarDeletingDestructor(1));
            self->field_0064 = 0;
        }
    }

    return result;
}

HOOK(0x00511920, C_008407a8_ReleaseOwnedScreens, "auto");
