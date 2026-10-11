#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

static int __fastcall C_008344f0_LoadAnimationEntryValues(C_008344f0* self, void* /*edx*/)
{
    int result = -1;
    if (self->field_0048 != 0xFFFFFFFFu && self->field_004c != 0xFFFFFFFFu)
    {
        int owner = GetValue8B0554();
        void* sceneSetup = CSF_AT(void*, owner, 0x1FC4);
        int entry = SceneMissionSetup_GetAnimationEntry(sceneSetup, nullptr, (int)self->field_0048);

        unsigned int index = 0;
        self->field_0058 = 0;
        int record = CSF_AT(int, entry, 0x24) + 44 * (int)self->field_004c;
        result = record;
        int begin = CSF_AT(int, record, 0x20);
        unsigned int count;
        if (begin != 0)
        {
            result = (CSF_AT(int, record, 0x24) - begin) >> 3;
            count = (unsigned int)result;
        }
        else
        {
            count = 0;
        }

        if (count != 0)
        {
            do
            {
                int elem = CSF_AT(int, CSF_AT(int, entry, 0x24) + 44 * (int)self->field_004c, 0x20) + 8 * (int)index;
                if (self->field_0058 >= CSF_AT(uint32_t, self, 0x54))
                {
                    S_0041e5f0_GrowArrayCapacity(
                        reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x50),
                        nullptr,
                        (int)CSF_AT(uint32_t, self, 0x5C));
                }
                uint32_t n = self->field_0058;
                uint32_t* slot = reinterpret_cast<uint32_t*>(self->field_0050) + n;
                self->field_0058 = n + 1;
                *slot = CSF_AT(uint32_t, elem, 4);
                result = (int)count;
                ++index;
            } while (index < count);
        }
    }
    return result;
}

HOOK(0x004A3670, C_008344f0_LoadAnimationEntryValues, "auto");
