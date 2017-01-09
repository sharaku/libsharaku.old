
all: check build deproy packageing

check:
	sh 01-static_check.sh

build:
	sh 02-build.sh

deproy:
	sh 03-deproy.sh

packageing:
	sh 04-package.sh

install:
	@tar zxvf deproy/libsharaku-0.0.0.tgz -C /

clean:
	@rm -fr result
	@rm -fr build
	@rm -fr deproy
