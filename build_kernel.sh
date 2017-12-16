#!/bin/bash

ZIMAGE_DIR=arch/arm/boot/zImage
MKBOOT_DIR=~/mkbootimg_tools/
KERNEL_DIR=${MKBOOT_DIR}/boot_org/kernel
BOOTIMAGE_DIR=~/build/kernel


build_kernel() {
    export ARCH=arm
    export CROSS_COMPILE="/home/dhyey/Desktop/liberty_tools/arm-eabi-4.8/bin/arm-eabi-"

    echo -e  "\n\nBuilding Kernel"

    ./clean_kernel.sh

    make exynos5422-k3g_00_defconfig
    make -j4

    if [ -e $ZIMAGE_DIR ]
    then
        mv $ZIMAGE_DIR $KERNEL_DIR
        cd $MKBOOT_DIR
        read -p "Enter Kernel Version: " version
        ./mkboot boot_org boot-${version}.img
        mv ${MKBOOT_DIR}/boot-${version}.img ${BOOTIMAGE_DIR}
    else
        echo -e "\n Error Building Kernel"
    fi
}

build_kernel

