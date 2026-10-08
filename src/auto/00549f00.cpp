#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, MenuVideoOptions_ConstructEmbeddedBase,
         0x00712CC0, (std::uint32_t* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());

static void** const g_MenuScreenVtable =
    reinterpret_cast<void**>(0x0083042C);
static void** const g_MenuVideoOptionsVtable =
    reinterpret_cast<void**>(0x00844148);
static void** const g_MenuVideoOptionsSecondaryVtable =
    reinterpret_cast<void**>(0x008440F8);

static CMenuVideoOptions* __fastcall CMenuVideoOptions_Construct(
    CMenuVideoOptions* self, void* /*edx*/, int argument)
{
    auto* embeddedBase = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(self) + 4);

    CSF_AT(void**, self, 0) = g_MenuScreenVtable;
    MenuVideoOptions_ConstructEmbeddedBase(embeddedBase, nullptr);

    CSF_AT(void**, self, 0) = g_MenuVideoOptionsVtable;
    self->field_0004 = reinterpret_cast<std::uintptr_t>(
        g_MenuVideoOptionsSecondaryVtable);

    self->field_0028 = 0;
    self->field_0028 = 800;
    self->field_002c = 0;
    self->field_002c = 600;
    self->field_0038 = 1;
    self->field_003c = 1;
    self->field_0044 = 1;
    self->field_0048 = 1;
    self->field_004c = 1;
    self->field_0050 = 1;
    self->field_0054 = 1;
    self->field_0030 = 2;
    self->field_0034 = 0.5f;
    self->field_0039 = 0;
    self->field_0040 = 2;
    self->field_0074 = static_cast<std::uint32_t>(argument);
    self->field_001c = 0;

    void* globalObject = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(
            static_cast<std::uint32_t>(GetGlobalValue_8B056C())));
    const std::uint32_t context = CSF_AT(std::uint32_t, globalObject, 0x18);
    self->field_0020 = context;

    std::uint32_t contextMember = 0;
    if (context != 0) {
        contextMember = CSF_AT(
            std::uint32_t,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(context)),
            0x0C);
    }

    void** vtable = CSF_AT(void**, self, 0);
    self->field_0024 = contextMember;
    self->field_0014 = 0;
    self->field_0078 = 0;
    self->field_0018 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_006c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_007c = 0;
    self->field_0070 = 0;

    ((void(__fastcall*)(void*, void*))vtable[0x40 / 4])(self, nullptr);
    return self;
}

HOOK(0x00549F00, CMenuVideoOptions_Construct, "auto");
