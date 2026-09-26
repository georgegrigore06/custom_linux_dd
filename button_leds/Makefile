DRIVER = my_driver
DEVICETREE = overlay
INCLUDE_COMMON=/usr/src/linux-headers-*-common-rpi/include
obj-m += ${DRIVER}.o

all: build
	sudo insmod ${DRIVER}.ko
	sudo dtoverlay -d . ${DEVICETREE}
	
remove: clean
	sudo rmmod ${DRIVER}
	sudo dtoverlay -r ${DEVICETREE}

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -E -nostdinc \
		-I ${INCLUDE_COMMON} \
		-undef -D__DTS__ -x assembler-with-cpp ${DEVICETREE}.dts -o ${DEVICETREE}.dts.preprocessed
	dtc -@ -I dts -O dtb -o ${DEVICETREE}.dtbo ${DEVICETREE}.dts.preprocessed

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf *.dtbo *.preprocessed