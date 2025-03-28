#pragma once

extern "C"
{
    #include <Base.h>
    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/SerialPortLib.h>
    #include <Library/PrintLib.h>
    #include <Library/UefiBootServicesTableLib.h>

    #include <sal.h>
}

#include <cstdint>

class Serial
{
public:
    EFI_STATUS
    EFIAPI
    Write(
        _Out_ const char* Format,
        ...
    );

private:
    EFI_SERIAL_IO_PROTOCOL* SerialIo = nullptr;
};