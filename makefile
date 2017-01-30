all: BootLoader Kernel32 Disk.img

BootLoader:
	@echo
	@echo ============== Build Boot Loader ==============
	@echo
	
	make -C 00.BootLoader
	
	@echo
	@echo =============== Build Complete ================
	@echo

Kernel32:
	@echo
	@echo ============== Build 32bit Kernel =============
	@echo
	
	make -C 01.Kernel32
	
	@echo
	@echo =============== Build Complete ================
	@echo

ImageMaker:
	@echo
	@echo ============== Build ImageMaker utility =============
	@echo
	
	make -C 04.Utility/00.ImageMaker
		
	@echo
	@echo ================== Build Complete ===================
	@echo

Disk.img: 00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin ImageMaker
	@echo
	@echo ============ Disk image Build Start ===========
	@echo
	
	04.Utility/00.ImageMaker/ImageMaker $^
	
	@echo
	@echo ============= All Build Complete ===============
	@echo

clean:
	make -C 00.BootLoader clean
	make -C 01.Kernel32 clean
	make -C 04.Utility/00.ImageMaker clean
	rm -f Disk.img

