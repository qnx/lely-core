# Testing CANopen ECSS (https://ecss.nl/) compliant port of lely-core on QNX

**NOTE**: QNX ports are only supported from a **Linux host** operating system

The lely-core libraries normally wants to be tested on the same machine it was built on. This obviously doesn't work when cross-compiling for QNX. The gist is to build, then copy the whole lely-core tests tree on a target. This will include all the relevant files and directory structure which lely-core libraries expect when running its test suite.

# Running the Test Suite

### Install dependencies

`sudo apt install automake libtool pkg-config`

### Generate GNU build tool ./configure and all needed Makefiles

`autoreconf -i`

### Setup QNX SDP environment

`source <path-to-sdp>/qnxsdp-env.sh`

### Build and install lely-core tests into SDP
*ECSS compliant port has to be tested by CTSSW - Test Suite*

`JLEVEL=$(nproc) CPULIST=x86_64 make -C qnx/build check`

### Then build your QNX image using mkqnximage and the following options

`export LELY_CORE_ROOT=$PWD`

`mkdir test_image && cd test_image`

`mkqnximage --extra-dirs=$LELY_CORE_ROOT/qnx/test/mkqnximage --clean --run --force --test-lely-core=$QNX_TARGET/x86_64/usr/bin/canopen_tests/lely-core`

### Once the target has booted, the lely-core tests will be located in /data/lely-core

`cd /data/lely-core`
    
`./run_testsuites.sh`

### Test execution summary

```
...
==========================================
Tests summary for lely-core 3.2.0
==========================================
# TOTAL: 8
# PASS: 8
# FAIL: 0
==========================================
```
