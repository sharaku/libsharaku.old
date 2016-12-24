# cross-build
===============

# はじめに
dockerにてx64ビルド、armビルド環境を提供するコンテナです。  
コンテナ内のファイルはホスト側からは隔離されます。永続的なファイルの保存が必要な場合は-vオプションを使用してホスト側のディレクトリをマウントしてください。

使い方
------
# Installation
以下のようにdocker imageをpullします。

    docker pull sharaku/cross-build

# Quick Start
cross-buildのimageを実行します。

    docker run --detach \
         --name build-server \
         --restart always \
         --volume /path/to/source:/path/to/container/data:rw \
         sharaku/cross-build

cross-buildに接続します。

    docker exec -it build-server /bin/bash

## Argument

+   `-v /path/to/source:/path/to/container/data:rw` :  
    永続的に保存するデータのディレクトリを指定します。任意の数の-vオプションを使用可能です。

# Installed environment
sharaku/cross-buildコンテナには以下がインストール済みです。

base:ubuntu:16.04

build tools:

    u-boot-tools, make, cmake, cppcheck
    build-essential, cppcheck
    gcc, g++
    gcc-arm-none-eabi
    gcc-arm-linux-*
    libboost-all-dev, libpython-all-dev
    python2.7.3(arm-linux-gnueabi), boost-1.61.0(arm-linux-gnueabi)

tools:

    wget, git, vim, bzip2, nkf, unzip, bc, qemu

利用例
------

Windowsからソースコードを編集し、armのビルドを行う方法です。
以下の条件でev3rt-buildを構築する例です。

+ docker動作ホストのIP：192.168.0.2
+ ボリューム（ホスト側）：/var/lib/samba
+ ボリューム（コンテナ側）：/srv

1.sambaコンテナを起動する。 (例では、jenserat/samba-publicshareを使用します)

    docker run -td \
         --name samba-server \
         --restart always \
         --publish 445:445 \
         --publish 137:137 \
         --publish 138:138 \
         --publish 139:139 \
         --volume /var/lib/samba:/srv \
         jenserat/samba-publicshare

2.sharaku/cross-buildコンテナを起動する。

    docker run --detach \
         --name build-server \
         --restart always \
         --volume /var/lib/samba:/srv:rw \
         sharaku/cross-build

3.Windowsからsambaコンテナへ接続

    \\192.168.0.2\public\

4.sharaku/cross-buildへ接続し、ディレクトリ変更。

    docker exec -it build-server /bin/bash
    cd /srv
