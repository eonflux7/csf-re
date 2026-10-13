#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, Alt_InitCategoryObject, 0x004D67C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083b580_InitWithUnitScale, 0x004CFD10, (C_0083b580* self, void* , int a2));
CSF_FUNC(int, __fastcall, C_0083ba40_Construct, 0x004D1C30, (C_0083ba40* self, void* , int a2));

// Creates the alternate-category object for the given category id (a1) and
// initializes it with a2. Returns the new object, or nullptr if the category
// is not handled here or the allocation failed.
static uint32_t* __cdecl TryCreateAlternateCategoryObject(int categoryId, int initArg)
{
    uint32_t* result = nullptr;

    switch (categoryId)
    {
    case 6:
    case 7:
    {
        void* obj = Mem_Alloc(96);
        if (obj)
            result = Alt_InitCategoryObject(obj, nullptr, initArg);
        break;
    }
    case 12:
    {
        C_0083ba40* obj = reinterpret_cast<C_0083ba40*>(Mem_Alloc(184));
        if (obj)
            result = reinterpret_cast<uint32_t*>(
                static_cast<intptr_t>(C_0083ba40_Construct(obj, nullptr, initArg)));
        break;
    }
    case 20:
    {
        C_0083b580* obj = reinterpret_cast<C_0083b580*>(Mem_Alloc(104));
        if (obj)
            result = reinterpret_cast<uint32_t*>(
                static_cast<intptr_t>(C_0083b580_InitWithUnitScale(obj, nullptr, initArg)));
        break;
    }
    default:
        return result;
    }
    return result;
}

HOOK(0x004DC440, TryCreateAlternateCategoryObject, "auto");
