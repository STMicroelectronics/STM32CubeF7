@echo off
echo AXF to BIN file generation ...

"C:\Keil_v5.17\ARM\ARMCC\bin\fromelf.exe" --bin --output ..\MDK-ARM\STM32F769I_EVAL\STM32F769I_EVAL_SysTick.bin ..\MDK-ARM\STM32F769I_EVAL\STM32F769I_EVAL.axf 

echo AXF to BIN file generation completed.

