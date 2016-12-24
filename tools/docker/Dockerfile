FROM ubuntu:16.04
MAINTAINER sharaku

# ############################################################################
# installation of build tools
RUN \
	apt-get update && \
	apt-get install -y \
		u-boot-tools \
		make \
		cmake \
		cppcheck \
		wget git vim bzip2 nkf unzip bc \
		qemu \
		g++ && \
	apt-get clean

# installation of gcc-arm-none-eabi
RUN \
	apt-get update && \
	apt-get install -y apt-file && \
	apt-file update && \
	apt-file search add-apt-repository && \
	apt-get install -y python-software-properties && \
	apt-get install -y software-properties-common && \
	apt-get update && \
	add-apt-repository ppa:team-gcc-arm-embedded/ppa && \
	apt-get update  && \
	apt-get install -y \
		gcc-arm-none-eabi=`apt-cache madison gcc-arm-none-eabi | grep "amd64" | cut -d'|' -f 2 | tr -d ' '` \
		binutils-arm-none-eabi && \
	apt-get clean

# installation of gcc-arm-linux-*
RUN \
	apt-get update && \
	apt-get install -y \
		gcc-arm-linux-gnueabihf \
		g++-arm-linux-gnueabihf \
		gcc-arm-linux-gnueabi \
		g++-arm-linux-gnueabi \
		binutils-arm-linux-gnueabihf \
		binutils-arm-linux-gnueabi && \
	apt-get clean

# installation of libs
RUN \
	apt-get update && \
	apt-get install -y \
		libboost-all-dev \
		libpython-all-dev && \
	apt-get clean

# jenkins client (java sdk)
RUN apt-get install -y default-jdk

# ############################################################################
# add start sh
ADD ./install/python_install.sh /opt/install/python_install.sh
ADD ./install/boost_install.sh /opt/install/boost_install.sh
RUN \
	chmod 555 /opt/install/python_install.sh && \
	chmod 555 /opt/install/boost_install.sh && \
	/opt/install/python_install.sh && \
	/opt/install/boost_install.sh

CMD ["tail","-f","/dev/null"]
