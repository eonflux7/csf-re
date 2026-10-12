#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0085ebf4_Construct, 0x0069FBA0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, C_0085ec18_Construct, 0x006A0010, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0085ec34_Construct, 0x006A0260, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwResourcesSetArenaSize, 0x007C5F60, (int a1));

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

static char __cdecl CoreVariantValues_InitializeFileSystems(int context)
{
    if (RwResourcesSetArenaSize(-1) == 0)
        return 0;

    void* storage = Mem_Alloc(12);
    auto* first = storage
        ? reinterpret_cast<C_0085ec34*>(C_0085ec34_Construct(storage, nullptr))
        : nullptr;

    void* options = g_pOptions;
    CSF_AT(C_0085ec34*, options, 0x48) = first;
    if (!first->vSlot4(2, CSF_AT(uint8_t, options, 0), context))
        return 0;

    storage = Mem_Alloc(8);
    auto* second = storage
        ? reinterpret_cast<C_0085ec18*>(C_0085ec18_Construct(storage, nullptr))
        : nullptr;

    options = g_pOptions;
    CSF_AT(C_0085ec18*, options, 0x44) = second;
    if (!second->vSlot4(4, CSF_AT(uint8_t, options, 0), context))
        return 0;

    storage = Mem_Alloc(4);
    uint32_t* third = storage
        ? C_0085ebf4_Construct(storage, nullptr)
        : nullptr;

    options = g_pOptions;
    CSF_AT(uint32_t*, options, 0x4C) = third;
    char result = reinterpret_cast<char(__fastcall*)(void*, void*, int, int, int)>(
        CSF_AT(void**, third, 0)[0x4 / 4])(
            third, nullptr, 2, CSF_AT(uint8_t, options, 0), context);

    if (result)
        CSF_AT(C_0085ec18*, g_pOptions, 0x44)->vApplyStoredDefaultFileSystem();

    return result;
}

HOOK(0x006971B0, CoreVariantValues_InitializeFileSystems, "auto");
