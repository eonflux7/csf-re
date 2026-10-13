#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00865758*, __fastcall, C_00865758_Init, 0x0072A140, (C_00865758* self, void*));
CSF_FUNC(C_008657e0*, __fastcall, C_008657e0_Initialize, 0x0072A170, (C_008657e0* self, void*));
CSF_FUNC(C_00865870*, __fastcall, C_00865870_Initialize, 0x0072A1B0, (C_00865870* self, void*));
CSF_FUNC(C_008658f8*, __fastcall, C_008658f8_Construct, 0x0072A1F0, (C_008658f8* self, void*));
CSF_FUNC(C_00865aa8*, __fastcall, C_00865aa8_Construct, 0x0072A220, (C_00865aa8* self, void*));
CSF_FUNC(C_00865980*, __fastcall, C_00865980_Construct, 0x0072A2C0, (C_00865980* self, void*));
CSF_FUNC(C_00865a10*, __fastcall, C_00865a10_Initialize, 0x0072A2F0, (C_00865a10* self, void*));
CSF_FUNC(C_008656d0*, __fastcall, C_008656d0_Construct, 0x0072A340, (C_008656d0* self, void*));

static std::uint32_t* __cdecl GuiEffects_Create(int effectType)
{
    std::uint32_t* effect = nullptr;

    switch (effectType)
    {
    case 0:
    {
        auto* storage = reinterpret_cast<C_008658f8*>(CoreMemory_AllocUntagged(60));
        if (!storage)
            return nullptr;
        effect = reinterpret_cast<std::uint32_t*>(C_008658f8_Construct(storage, nullptr));
        break;
    }
    case 1:
    {
        auto* storage = reinterpret_cast<C_00865758*>(CoreMemory_AllocUntagged(56));
        if (!storage)
            return nullptr;
        effect = reinterpret_cast<std::uint32_t*>(C_00865758_Init(storage, nullptr));
        break;
    }
    case 2:
    {
        auto* storage = reinterpret_cast<C_008657e0*>(CoreMemory_AllocUntagged(84));
        if (!storage)
            return nullptr;
        effect = reinterpret_cast<std::uint32_t*>(C_008657e0_Initialize(storage, nullptr));
        break;
    }
    case 3:
    {
        auto* storage = reinterpret_cast<C_00865870*>(CoreMemory_AllocUntagged(76));
        if (!storage)
            return nullptr;
        effect = reinterpret_cast<std::uint32_t*>(C_00865870_Initialize(storage, nullptr));
        break;
    }
    case 4:
    {
        auto* storage = reinterpret_cast<C_00865980*>(CoreMemory_AllocUntagged(72));
        if (!storage)
            return nullptr;
        effect = reinterpret_cast<std::uint32_t*>(C_00865980_Construct(storage, nullptr));
        break;
    }
    case 5:
    {
        auto* storage = reinterpret_cast<C_008656d0*>(CoreMemory_AllocUntagged(52));
        if (!storage)
            return nullptr;
        effect = reinterpret_cast<std::uint32_t*>(C_008656d0_Construct(storage, nullptr));
        break;
    }
    case 6:
    {
        auto* storage = reinterpret_cast<C_00865aa8*>(CoreMemory_AllocUntagged(104));
        if (!storage)
            return nullptr;
        effect = reinterpret_cast<std::uint32_t*>(C_00865aa8_Construct(storage, nullptr));
        break;
    }
    case 7:
    {
        auto* storage = reinterpret_cast<C_00865a10*>(CoreMemory_AllocUntagged(96));
        if (!storage)
            return nullptr;
        effect = reinterpret_cast<std::uint32_t*>(C_00865a10_Initialize(storage, nullptr));
        break;
    }
    default:
        return nullptr;
    }

    if (effect)
    {
        void** vtable = CSF_AT(void**, effect, 0);
        CSF_AT(std::uint32_t, effect, 0x08) = 0;
        CSF_AT(std::uint32_t, effect, 0x04) = 0;
        ((void(__fastcall*)(void*, void*, int))vtable[0x1C / 4])(
            effect, nullptr, 1);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, effect, 0)[0x08 / 4])(
            effect, nullptr, effectType);
    }

    return effect;
}

HOOK(0x00713B20, GuiEffects_Create, "auto");
