#include "HelloWorldApp.hpp"

int
EFIAPI
Main(
    [[maybe_unused]] _In_ int ArgC,
    [[maybe_unused]] _In_ char* ArgV[]
)
{
    Serial SerialPort;
    
    for (int i = 0; i < ArgC; i++)
    {
        SerialPort.Write("%a\n", ArgV[i]);
    }

    return 0;
}