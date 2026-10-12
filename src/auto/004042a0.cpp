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

CSF_FUNC(int, __cdecl, GfxSkin_GetDistanceOverrideOwner, 0x006B8840, (int a1));
CSF_FUNC(int, __fastcall, DistanceOwnerList_GetCount, 0x004022C0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, DistanceOwnerList_Append, 0x00404230, (void* self, void* edx, uint32_t* a3));

// Registers the distance-override owner of skin a3 into the list at a5[1],
// unless it is already present. Always returns a3.
static int __cdecl GfxSkin_RegisterDistanceOverride(int a1, int a2, int a3, int a4, uint32_t* a5)
{
    int ownerObject = GfxSkin_GetDistanceOverrideOwner(a3);
    if (ownerObject)
    {
        uint32_t candidate = CSF_AT(uint32_t, reinterpret_cast<void*>(ownerObject), 0x10);
        if (candidate != 0 && candidate != a5[0])
        {
            void* obj = reinterpret_cast<void*>(candidate);
            // vtable slot 0x40 / 4 = 16
            bool canRegister = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[16])(obj, nullptr);
            if (canRegister)
            {
                // vtable slot 0x70 / 4 = 28
                int infoObject = ((int(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[28])(obj, nullptr);
                if (infoObject && CSF_AT(uint32_t, reinterpret_cast<void*>(infoObject), 4))
                {
                    void* list = reinterpret_cast<void*>(a5[1]);
                    uint32_t count = static_cast<uint32_t>(DistanceOwnerList_GetCount(list, nullptr));
                    bool alreadyListed = false;
                    if (count != 0)
                    {
                        uint32_t* items = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, list, 4));
                        for (uint32_t i = 0; i < count; ++i)
                        {
                            if (items[i] == candidate)
                            {
                                alreadyListed = true;
                                break;
                            }
                        }
                    }
                    if (!alreadyListed)
                    {
                        DistanceOwnerList_Append(list, nullptr, &candidate);
                    }
                }
            }
        }
    }
    return a3;
}

HOOK(0x4042A0, GfxSkin_RegisterDistanceOverride, "auto");
