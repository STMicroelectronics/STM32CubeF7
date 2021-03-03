@echo off
echo AXF to BIN file generation ...

"C:\Keil_v5.17\ARM\ARMCC\bin\fromelf.exe" --bin --output ..\MDK-ARM\STM327x6G_EVAL\STM327x6G_EVAL_SysTick.bin ..\MDK-ARM\STM327x6G_EVAL\STM327x6G_EVAL.axf 

echo AXF to BIN file generation completed.

