#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static void __fastcall CMenuMessageBox_DestroyOwnedObject(
    CMenuMessageBox* self, void* /*edx*/)
{
    void* ownedObject = reinterpret_cast<void*>(self->field_0894);
    if (ownedObject != nullptr) {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0)[0])(
                ownedObject, nullptr, 1);

        self->field_0894 = 0;
        self->field_002c = 0;
        self->field_0030 = 0;
        self->field_0034 = 0;
        self->field_0038 = 0;
        self->field_003c = 0;
        self->field_0040 = 0;
        self->field_0044 = 0;
        self->field_0048 = 0;
        self->field_004c = 0;
        self->field_0050 = 0;
        self->field_0054 = 0;
        self->m_staticControl = 0;
        self->field_005c = 0;
        self->field_0060 = 0;
        self->field_006c = 0;
        self->field_0064 = 0;
        self->field_0070 = 0;
        self->field_0068 = 0;
        self->field_0074 = 0;
    }
}

HOOK(0x00528A20, CMenuMessageBox_DestroyOwnedObject, "auto");
