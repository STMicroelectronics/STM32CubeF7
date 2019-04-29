@echo off
echo AXF to BIN file generation ...

"C:\Keil_v5.17\ARM\ARMCC\bin\fromelf.exe" --bin --output ..\MDK-ARM\FLASH_DualBoot_Bank2\FLASH_DualBoot_Bank2.bin ..\MDK-ARM\FLASH_DualBoot_Bank2\FLASH_DualBoot_Bank2.axf 

echo AXF to BIN file generation completed.

