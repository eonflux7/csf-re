#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, SubObject_InitBase, 0x006CA660, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00862240_Construct, 0x006F0040, (void* self, void* edx));
CSF_FUNC(std::uint32_t*, __fastcall, C_00862490_Initialize, 0x006F1A00, (C_00862490* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_00862598_Construct, 0x006F2AE0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00862640_Construct, 0x006F2D90, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00862748_Construct, 0x006F4CD0, (C_00862748* self, void*));

static std::uint32_t* __cdecl CreateParticleObject(int type)
{
    switch (type)
    {
    case 0:
    {
        auto* object = reinterpret_cast<C_00862748*>(CoreMemory_AllocUntagged(136));
        if (!object)
            return nullptr;
        return C_00862748_Construct(object, nullptr);
    }

    case 1:
    {
        auto* object = reinterpret_cast<C_00860a30*>(CoreMemory_AllocUntagged(152));
        if (!object)
            return nullptr;

        SubObject_InitBase(object, nullptr);
        object->vtbl = CSF_VTBL(0x00861508);
        object->field_0004 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00861500));
        object->field_0008 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x008614E4));
        S00402350_InitTransform(
            reinterpret_cast<S_00402350*>(reinterpret_cast<std::uint8_t*>(object) + 0x20),
            nullptr);

        CSF_AT(std::uint32_t, object, 0x5C) = 0;
        CSF_AT(std::uint32_t, object, 0x60) = 0;
        CSF_AT(std::uint32_t, object, 0x64) = 0;
        CSF_AT(std::uint32_t, object, 0x68) = 0;
        CSF_AT(std::uint32_t, object, 0x6C) = 0;
        CSF_AT(std::uint32_t, object, 0x70) = 0;
        CSF_AT(std::uint32_t, object, 0x74) = 0;
        CSF_AT(std::uint32_t, object, 0x78) = 0;
        CSF_AT(std::uint32_t, object, 0x7C) = 0;
        CSF_AT(std::uint32_t, object, 0x88) = 0;
        CSF_AT(std::uint32_t, object, 0x8C) = 0;
        CSF_AT(std::uint32_t, object, 0x90) = 0;
        CSF_AT(std::uint32_t, object, 0x94) = 0;
        return reinterpret_cast<std::uint32_t*>(object);
    }

    case 2:
    {
        auto* object = reinterpret_cast<C_00862640*>(CoreMemory_AllocUntagged(84));
        if (!object)
            return nullptr;
        return C_00862640_Construct(object, nullptr);
    }

    case 3:
    {
        auto* object = reinterpret_cast<C_00862598*>(CoreMemory_AllocUntagged(84));
        if (!object)
            return nullptr;
        return C_00862598_Construct(object, nullptr);
    }

    case 4:
    {
        auto* object = reinterpret_cast<C_00862490*>(CoreMemory_AllocUntagged(144));
        if (!object)
            return nullptr;
        return C_00862490_Initialize(object, nullptr);
    }

    case 5:
    {
        auto* object = reinterpret_cast<C_00860a30*>(CoreMemory_AllocUntagged(60));
        if (!object)
            return nullptr;

        SubObject_InitBase(object, nullptr);
        CSF_AT(std::uint32_t, object, 0x20) = 0;
        CSF_AT(std::uint32_t, object, 0x24) = 0;
        CSF_AT(std::uint32_t, object, 0x28) = 0;
        CSF_AT(std::uint32_t, object, 0x2C) = 0;
        CSF_AT(std::uint32_t, object, 0x34) = 0;
        CSF_AT(std::uint32_t, object, 0x38) = 0;
        CSF_AT(std::uint32_t, object, 0x30) = 0;
        object->vtbl = CSF_VTBL(0x00862390);
        object->field_0004 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00862384));
        object->field_0008 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00862368));
        return reinterpret_cast<std::uint32_t*>(object);
    }

    case 6:
    {
        auto* object = reinterpret_cast<C_00862240*>(CoreMemory_AllocUntagged(208));
        if (!object)
            return nullptr;
        return C_00862240_Construct(object, nullptr);
    }

    case 7:
    {
        auto* object = reinterpret_cast<C_00860a30*>(CoreMemory_AllocUntagged(64));
        if (!object)
            return nullptr;

        SubObject_InitBase(object, nullptr);
        object->vtbl = CSF_VTBL(0x008622E8);
        object->field_0004 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x008622E0));
        object->field_0008 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x008622C4));
        CSF_AT(std::uint32_t, object, 0x28) = 0;
        CSF_AT(std::uint32_t, object, 0x2C) = 0;
        CSF_AT(std::uint32_t, object, 0x30) = 0;
        CSF_AT(std::uint32_t, object, 0x3C) = 0;
        CSF_AT(std::int32_t, object, 0x38) = -1;
        CSF_AT(std::uint32_t, object, 0x20) = 0;
        CSF_AT(std::uint32_t, object, 0x24) = 0;
        CSF_AT(float, object, 0x34) = 1.0f;
        return reinterpret_cast<std::uint32_t*>(object);
    }

    default:
        return nullptr;
    }
}

HOOK(0x006C9BE0, CreateParticleObject, "auto");
