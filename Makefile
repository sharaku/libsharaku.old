# Copyright 2004-2016 abe takafumi All Rights Reserved.

lib-platform	= linux

all:
	$(MAKE) -C system/ lib-target=libsharaku-sys.a lib-platform=$(lib-platform)
	$(MAKE) -C devices/ lib-target=libsharaku-dev.a lib-platform=$(lib-platform)
	$(MAKE) -C modules/ lib-target=libsharaku-mod.a lib-platform=$(lib-platform)

clean:
	$(MAKE) -C system/ clean lib-target=libsharaku-sys.a 
	$(MAKE) -C devices/ clean lib-target=libsharaku-dev.a
	$(MAKE) -C modules/ clean lib-target=libsharaku-mod.a

