#pragma once

extern "C"
{
    #include <Base.h>
    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/UefiBootServicesTableLib.h>
    #include <Library/BaseMemoryLib.h>

    #include <sal.h>
}

#include <Library/UtilLib.hpp>

#include <cstdint>

namespace Hook
{
    EFI_STATUS
    EFIAPI
    HookJump(
        _In_ void* Address,
        _In_ void* JumpTo,
        _In_ bool IsReadOnly
    );

    EFI_STATUS
    EFIAPI
    HookCall(
        _In_ void* Address,
        _In_ void* CallAddress,
        _In_ bool IsReadOnly
    );
}