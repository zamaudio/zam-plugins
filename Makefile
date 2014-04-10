all:
	$(MAKE) -C libs/dgl
	$(MAKE) -C plugins/ZamComp/LV2/
	$(MAKE) -C plugins/ZamComp/LV2-UI/
	$(MAKE) -C utils/lv2-ttl-generator

	./utils/lv2_ttl_generator bin/lv2/ZamComp.lv2/ZamComp.so
	mv *.ttl bin/lv2/ZamComp.lv2

clean:
	$(MAKE) -C libs/dgl clean
	$(MAKE) -C plugins/ZamComp/LV2/ clean
	$(MAKE) -C plugins/ZamComp/LV2-UI/ clean
	rm -f utils/lv2_ttl_generator
