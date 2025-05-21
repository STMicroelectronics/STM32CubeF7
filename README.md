# STM32CubeF7 MCU Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/STM32CubeF7.svg?color=brightgreen)

> [!IMPORTANT]
> This repository has been created using the `git submodule` command. Please refer to the ["How to use"](README.md#how-to-use) section for more details.

## Overview

**STM32Cube** is an STMicroelectronics original initiative to ease developers' life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform delivered for each STM32 series.
   * The CMSIS modules (core and device) corresponding to the ARM(tm) core implemented in this STM32 product.
   * The STM32 HAL-LL drivers, an abstraction layer offering a set of APIs ensuring maximized portability across the STM32 portfolio.
   * The BSP drivers of each evaluation, discovery or nucleo board provided for this STM32 series.
   * A consistent set of middleware libraries such as RTOS, USB, FatFS, graphics, touch sensing library...
   * A full set of software projects (basic examples, applications, and demonstrations) for each board provided for this STM32 series.

The **STM32CubeF7 MCU Package** projects are directly running on the STM32F7 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications/Demonstration/Examples).

> [!NOTE]
> ## Some middleware libraries and projects are unavailable in this repository
> 
> In this repository, the middleware libraries listed below **along with** [this](Projects/README.md#list-of-unavailable-projects) list of projects (demos, applications, and examples) using them, are **not available** as they (the middleware libraries) are subject to some restrictive license terms requiring the user's approval via a "click thru" procedure.
> * `./Middlewares/ST/STM32_Audio`
> * `./Middlewares/ST/STemWin`
> * `./Middlewares/ST/TouchGFX`
> 
> If needed, they can be found inside the full firmware package available on our website `st.com` and downloadable from [here](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef7.html#get-software). You will be prompted to login or to register in case you have no account.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Release_Notes.html).

## How to use

This repository has been created using the `git submodule` command. Please check the instructions below for proper use. Please check also **the notes at the end of this section** for further information.

1. To **clone** this repository along with the linked submodules, option `--recursive` has to be specified as shown below.

```bash
git clone --recursive https://github.com/STMicroelectronics/STM32CubeF7.git
```

2. To get the **latest updates**, in case this repository is **already** on your local machine, issue the following **two** commands (with this repository as the **current working directory**).

```bash
git pull
git submodule update --init --recursive
```

3. To use the **same firmware version** as the one available on [st.com](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html), issue the command below **after** specifying the targeted `vX.Y.Z` version. This should be done **after** the command(s) indicated in instruction (1) or in instruction (2) above have been successfully executed.

```bash
git checkout vX.Y.Z # Specify the targeted vX.Y.Z version
```

4. To **avoid** going through the above instructions and **directly** clone the same firmware version as the one available on [st.com](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html), issue the command below **after** specifying the targeted `vX.Y.Z` version.

```bash
git clone --recursive  --depth 1 --branch vX.Y.Z https://github.com/STMicroelectronics/STM32CubeF7.git
```

> [!NOTE]
> * The latest version of this firmware available on GitHub may be **ahead** of the one available on [st.com](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html) or via [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html). This is due to the **rolling release** process deployed on GitHub. Please refer to [this](https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer/discussions/21) post for more details.
> * Option `--depth 1` specified in instruction (4) above is **not** mandatory. It may be useful to skip downloading all previous commits up to the one corresponding to the targeted version.
> * If GitHub "Download ZIP" option is used instead of the `git clone` command, then the different submodules have to be collected and added **manually**.

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
