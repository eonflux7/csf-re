#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Vtable addresses written by the constructor, stored as the exact immediates the original uses.
static constexpr uint32_t kVtblCMenuScreenBase = 0x0083042C;
static constexpr uint32_t kVtblCMenuScreenSecondary = 0x008453E8;
static constexpr uint32_t kVtblCMenuAccountEdit = 0x00845438;

// Callees (all __thiscall, plain ret).
CSF_FUNC(void*, __fastcall, MenuScreenBase_Construct, 0x00712CC0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuMemberObject_Construct, 0x005188D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMemberObject_Reset, 0x00518890, (void* self, void* edx));

static void* __fastcall CMenuAccountEdit_Construct(CMenuAccountEdit* self, void* /*edx*/, int ownerArg)
{
    // Base CMenuScreen sub-object at +0x04.
    void* baseSubObject = reinterpret_cast<uint8_t*>(self) + 0x04;
    CSF_AT(uint32_t, self, 0) = kVtblCMenuScreenBase;
    MenuScreenBase_Construct(baseSubObject, nullptr);
    CSF_AT(uint32_t, baseSubObject, 0) = kVtblCMenuScreenSecondary;

    // Member sub-object at +0x34.
    void* memberSubObject = reinterpret_cast<uint8_t*>(self) + 0x34;
    CSF_AT(uint32_t, self, 0) = kVtblCMenuAccountEdit;
    MenuMemberObject_Construct(memberSubObject, nullptr);

    self->field_0288 = static_cast<uint32_t>(ownerArg);
    self->field_0284 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0031 = 0;

    MenuMemberObject_Reset(memberSubObject, nullptr);

    self->field_0280 = 0;
    // Virtual call through the object's vtable, slot 0x40 / 4 = 16.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[16])(self, nullptr);

    return self;
}

HOOK(0x0055C950, CMenuAccountEdit_Construct, "auto");
