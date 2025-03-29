#pragma once

extern "C"
{
    #include <Base.h>
    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/UefiBootServicesTableLib.h>
    #include <Library/BaseMemoryLib.h>

    #include <IndustryStandard/PeImage.h>

    #include <sal.h>
}

#include <cstdint>

namespace Util
{
    void*
    EFIAPI
    GetImageBase(
        _In_ void* Address
    );

    uint64_t
    EFIAPI
    GetImageSize(
        _In_ void* ImageBase
    );

    uint64_t
    EFIAPI
    FindPattern(
        _In_ void* BaseAddress,
        _In_ uint64_t Size,
        _In_ const char* Pattern
    );

    void*
    EFIAPI
    CopyToReadOnly(
        _In_ void* Destination,
        _In_ const void* Source,
        _In_ const uint64_t Size
    );
}