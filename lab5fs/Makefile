BUILD_PATH = /lib/modules/2.6.9lab5/build
obj-m += lab5fs.o
all:
	make -C $(BUILD_PATH) M=$(PWD) modules
clean:
	make -C $(BUILD_PATH) M=$(PWD) clean
mount:
	# We put a — in front of the rmmod command to tell make to ignore
	# an error in case the module isn’t loaded.
	# -sudo rmmod lkm_example
	# Clear the kernel log without echo
	#sudo dmesg -c
	
	# create mock block device
	dd if=/dev/zero of=image bs=1024 count=1024
	
	# Insert the module
	insmod lab5fs.ko
	
	# mount the fs
	#mount -t lab5fs image /mnt -o loop
	
	# Display the kernel log
	#dmesg
	
unmnt:
	# umount /mnt
	rmmod lab5fs
