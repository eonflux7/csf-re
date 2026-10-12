#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SetFlagAndDispatchOnce, 0x00407450, (void* self, int a2, int a3, char a4, char a5));
CSF_FUNC(int, __cdecl, MissionScript_CreateValueForType, 0x004723D0, (int a1));
CSF_FUNC(int, __fastcall, MissionScript_AssignValueReference, 0x00473EB0, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, MissionScript_ProcessValueContents, 0x00473F00, (void* self, void* edx, void* a1));

static int __fastcall C_008313f8_ProcessValue(
    C_008313f8* self, void* /*edx*/, int a2)
{
    void* context = reinterpret_cast<void*>(a2);

    alignas(8) uint8_t referenceBytes[0x4];
    auto& valueReference =
        *reinterpret_cast<C_008313f8**>(referenceBytes);
    valueReference = self;

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x0) == 0)
    {
        void* stream = CSF_AT(void*, context, 0x4);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
                       CSF_AT(void**, stream, 0)[0x80 / 4])(
            stream, nullptr, 0, context);
    }

    const int savedState = CSF_AT(int, context, 0x1C);
    CSF_AT(uint8_t, context, 0x0) = failed;
    CSF_AT(int, context, 0x1C) = 0;

    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;

        auto* referenceId = &CSF_AT(C_00833908*, self, 0x8);
        failed = ((int(__fastcall*)(void*, void*, C_00833908**, void*))
                      CSF_AT(void**, stream, 0)[0x48 / 4])(
                     stream, nullptr, referenceId, context) == -1;
        CSF_AT(uint8_t, context, 0x0) = failed;

        if (!failed &&
            *referenceId == reinterpret_cast<C_00833908*>(-1))
        {
            C_008313f8* value = valueReference;
            if (CSF_AT(uint8_t, context, 0x14) != 0)
            {
                a2 = CSF_AT(uint8_t,
                            reinterpret_cast<void*>(value->field_0004), 0);
            }

            stream = CSF_AT(void*, context, 0x4);
            CSF_AT(int, context, 0x1C) = 0;
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;

            failed = ((int(__fastcall*)(void*, void*, int*, void*))
                          CSF_AT(void**, stream, 0)[0x58 / 4])(
                         stream, nullptr, &a2, context) == -1;
            CSF_AT(uint8_t, context, 0x0) = failed;

            if (!failed)
            {
                if (a2 == 1)
                {
                    if (CSF_AT(uint8_t, context, 0x14) != 0)
                    {
                        valueReference = CSF_AT(
                            C_008313f8*,
                            reinterpret_cast<void*>(value->field_0004), 0x4);
                    }

                    SetFlagAndDispatchOnce(
                        context, reinterpret_cast<int>(&valueReference),
                        0, 0, 0);

                    if (CSF_AT(uint8_t, context, 0x14) == 0 &&
                        CSF_AT(uint8_t, context, 0x0) == 0)
                    {
                        MissionScript_AssignValueReference(
                            value, nullptr,
                            reinterpret_cast<int>(valueReference));
                    }
                }
                else if (CSF_AT(uint8_t, context, 0x14) == 0)
                {
                    value->field_0004 = MissionScript_CreateValueForType(a2);
                }

                MissionScript_ProcessValueContents(value, nullptr, context);
            }
        }
    }

    const bool succeeded = CSF_AT(uint8_t, context, 0x0) == 0;
    CSF_AT(int, context, 0x1C) = savedState;

    if (succeeded)
    {
        void* stream = CSF_AT(void*, context, 0x4);
        const int result =
            ((int(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x0) = 1;
    return 1;
}

HOOK(0x00474250, C_008313f8_ProcessValue, "auto");
