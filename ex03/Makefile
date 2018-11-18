.PHONY: init build test rebuild-driver flash-driver rebuild-game flash-game

init:
	ptxdist images
	ptxdist test flash-all

build:
	ptxdist clean game
	ptxdist clean driver-gamepad
	ptxdist go
	ptxdist image root.romfs
	ptxdist test flash-rootfs

test:
	make build
	miniterm.py -b 115200 -p /dev/ttyS0

rootfs:
	ptxdist image root.romfs > /dev/null
	ptxdist test flash-rootfs > /dev/null

rebuild-driver:
	ptxdist clean driver-gamepad > /dev/null
	ptxdist compile driver-gamepad > /dev/null

flash-driver:
	make rebuild-driver
	ptxdist targetinstall driver-gamepad > /dev/null
	ptxdist targetinstall kernel > /dev/null
	make rootfs

rebuild-game:
	ptxdist clean game > /dev/null
	ptxdist compile game > /dev/null

flash-game:
	make rebuild-game
	ptxdist targetinstall game > /dev/null
	make rootfs
	