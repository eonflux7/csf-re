#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));

static unsigned int __fastcall UpdateAvailabilityFlag(void* self, void* /*edx*/)
{
    bool available = false;

    if (((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr)
        && CSF_AT(C_0082dd78*, self, 0x4C)
        && C_0082dd78_DispatchReturnedObject(
            CSF_AT(C_0082dd78*, self, 0x4C), nullptr))
    {
        const int returnedObject = C_0082dd78_DispatchReturnedObject(
            CSF_AT(C_0082dd78*, self, 0x4C), nullptr);
        const int objectValue = CSF_AT(int, reinterpret_cast<void*>(returnedObject), 0x4);

        if (!((bool (__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0x7C / 4])(self, nullptr))
        {
            void* object = ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);

            if (((bool (__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x38 / 4])(object, nullptr))
            {
                object = ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);

                if (((bool (__fastcall*)(void*, void*))
                        CSF_AT(void**, object, 0)[0x44 / 4])(object, nullptr)
                    && objectValue != 0)
                {
                    available = true;
                }
            }
        }
    }

    unsigned int flags = CSF_AT(unsigned int, self, 0x170);
    flags = available ? flags | 8 : flags & 0xFFFFFFF7;
    CSF_AT(unsigned int, self, 0x170) = flags;
    return flags;
}

HOOK(0x0048FA70, UpdateAvailabilityFlag, "auto");
