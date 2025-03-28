# **ShmurkioPkg**  

ShmurkioPkg is an EDK2 package that provides useful libraries for UEFI development.  

## **Included Libraries**  

### **1. EntryLib**

EntryLib provides an enhanced entry point with **ArgC and ArgV support**:

```cpp
int
EFIAPI
Main(
    [[maybe_unused]] _In_ int ArgC,
    [[maybe_unused]] _In_ char* ArgV[]
)
```

*Note: In EDK2 drivers, the only argument passed is their file path.*  

### **2. SerialLib**  

SerialLib enables simple debugging via the **serial port**:

```cpp
Serial SerialPort;
    
for (int i = 0; i < ArgC; i++)
{
    SerialPort.Write("%a\n", ArgV[i]);
}
```

![SerialLib](https://i.imgur.com/T1ckGF2.png)

## **Example Usage**  

For a practical example of how to use ShmurkioPkg, check out the HelloWorldPkg.