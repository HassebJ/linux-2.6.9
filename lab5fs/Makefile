BUILD_PATH = /lib/modules/2.6.9lab5/build
obj-m += lab5fs.o
all:	mklab5fs
	make -C $(BUILD_PATH) M=$(PWD) modules
	
clean:
	make -C $(BUILD_PATH) M=$(PWD) clean
	rm mklab5fs
mount:
	# We put a — in front of the rmmod command to tell make to ignore
	# an error in case the module isn’t loaded.
	-rmmod lab5fs
	# Clear the kernel log without echo
	-dmesg -c > /dev/null
	
	# create mock block device
	#dd if=/dev/zero of=image bs=1024 count=1024
	
	# Insert the module
	insmod lab5fs.ko
	
	# mount the fs
	mount -t lab5fs image p -o loop
	
	# Display the kernel log
	dmesg
	
mklab5fs:
	gcc -g -o mklab5fs mklab5fs.c
	
unmount:
	umount p
	rmmod lab5fs
