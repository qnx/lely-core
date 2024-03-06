# Compile the port for QNX

**NOTE**: QNX ports are only supported from a **Linux host** operating system

### Install dependencies

`sudo apt install automake libtool pkg-config`

### Generate GNU build tool ./configure and all needed Makefiles

`autoreconf -i`

### Setup QNX SDP environment

`source <path-to-sdp>/qnxsdp-env.sh`

### Build and install lely-core into SDP

`JLEVEL=$(nproc) make -C qnx/build install`

### Build and install ECSS (https://ecss.nl/) compliant port of lely-core into SDP

`JLEVEL=$(nproc) ECSS=on make -C qnx/build install`

**All lely core headers have to be installed to SDP**
* $QNX_TARGET/usr/include/lely/

**All lely core libraries have to be installed to SDP**
* $QNX_TARGET/x86_64/usr/lib/
* $QNX_TARGET/aarch64le/usr/lib/
