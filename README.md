# STM32CubeF7 MCU Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/STM32CubeF7.svg?color=brightgreen)

**STM32Cube** is an STMicroelectronics original initiative to ease developers' life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform delivered for each STM32 series.
   * The CMSIS modules (core and device) corresponding to the ARM(tm) core implemented in this STM32 product.
   * The STM32 HAL-LL drivers, an abstraction layer offering a set of APIs ensuring maximized portability across the STM32 portfolio.
   * The BSP drivers of each evaluation, demonstration or nucleo board provided for this STM32 series.
   * A consistent set of middleware libraries such as RTOS, USB, FatFS, graphics, touch sensing library...
   * A full set of software projects (basic examples, applications, and demonstrations) for each board provided for this STM32 series.

The **STM32CubeF7 MCU Package** projects are directly running on the STM32F7 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications/Demonstration/Examples).

## Some middleware libraries are unavailable in this repository

In this repository, the middleware libraries listed below **along with** the projects (demos, applications, and examples) using them, are **not available** as they (the middleware libraries) are subject to some restrictive license terms requiring the user's approval via a "click thru" procedure.
* `./Middlewares/ST/STM32_Audio`
* `./Middlewares/ST/STemWin`
* `./Middlewares/ST/TouchGFX`

If needed, they can be found inside the full firmware package available on our website `st.com` and downloadable from [here](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef7.html#get-software). You will be prompted to login or to register in case you have no account.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Release_Notes.html).

## Boards available

  * STM32F7
    * [STM32F7508-DK](https://www.st.com/en/evaluation-tools/stm32f7508-dk.html)
    * [32F723EDISCOVERY](https://www.st.com/en/evaluation-tools/32f723ediscovery.html)
    * [32F746GDISCOVERY](https://www.st.com/en/evaluation-tools/32f746gdiscovery.html)
    * [32F769IDISCOVERY](https://www.st.com/en/evaluation-tools/32f769idiscovery.html)
    * [NUCLEO-F722ZE](https://www.st.com/en/evaluation-tools/nucleo-f722ze.html)
    * [NUCLEO-F746ZG](https://www.st.com/en/evaluation-tools/nucleo-f746zg.html)
    * [NUCLEO-F756ZG](https://www.st.com/en/evaluation-tools/nucleo-f756zg.html)
    * [NUCLEO-F767ZI](https://www.st.com/en/evaluation-tools/nucleo-f767zi.html)
    * [STM32746G-EVAL](https://www.st.com/en/evaluation-tools/stm32746g-eval.html)
    * [STM32756G-EVAL](https://www.st.com/en/evaluation-tools/stm32756g-eval.html)
    * [STM32F769I-EVAL](https://www.st.com/en/evaluation-tools/stm32f769i-eval.html)
    * [STM32F779I-EVAL](https://www.st.com/en/evaluation-tools/stm32f779i-eval.html)
    * [STM32F7308-DK](https://www.st.com/en/evaluation-tools/stm32f7308-dk.html)

## Troubleshooting

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.
