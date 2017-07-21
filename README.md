![logo](doc/logo.png)

CANopen library
===============

Introduction
------------

The Lely CANopen library ([liblely-co]) is a CANopen implementation for both
masters and slaves. Most of the functionality of [CiA] 301, 302, 305, 306, 309
and 315 is provided. The implementation is passive; it does not perform any I/O
or timing by itself. Instead it relies on the user to send and received CAN
frames and update the clock. This allows the library to be integrated into a
wide variety of frameworks and communication patterns. The library uses the
platform-independent CAN frame definition from the Lely CAN library
([liblely-can]) and can be used with the Lely I/O library ([liblely-io]).

Additionally, several tools are provided:
- can2udp - CAN to UDP forwarding tool
- cocat/cocatd - CANopen cat tools
- coctl - CANopen control tool
- dcf2c - CANopen EDS/DCF to C conversion tool

Download
--------

liblely-co is hosted on [GitLab]. You can clone the repository with

    $ git clone https://gitlab.com/lely_industries/co.git

or download the latest release
([v1.4.1](https://gitlab.com/lely_industries/co/tags/v1.4.1)).

Binary packages for Ubuntu 16.04 LTS (Xenial Xerus) can be found at
https://launchpad.net/~lely/+archive/ubuntu/ppa.

Build and install
-----------------

liblely-co uses the GNU Build System (`configure`, `make`, `make install`) and
has native support for POSIX platforms (Debian-based Linux distributions in
particular) and Windows (Vista and higher) through [Cygwin] or [MinGW-w64].
[GCC ARM Embedded] and Microsoft Visual Studio (2013 and higher) are also
supported, but project files will have to be created by hand.

The release archive includes the build system, but if you want to build a
checkout of the repository, you need to install the autotools (autoconf,
automake and libtool). The build system can then be generated by running

    $ autoreconf -i

in the root directory of the repository.

If you want to generate HTML documentation from the source code, you need to
install [doxygen] and [Graphviz].

liblely-co requires the Lely C11 and POSIX compatibility library
([liblely-libc]), the Lely utilities library ([liblely-util]), the Lely CAN
library ([liblely-can]) and, for some of the tools, the Lely I/O library
([liblely-io]) to be installed.

### Debian packages

For Debian-based Linux distributions, the preferred installation method is to
generate and install Debian packages. Install the Debian package build system
with

    $ sudo apt-get install cdbs devscripts

The packages can now be built by running

    $ debuild -uc -us -b

from the root directory of the project. Once the packages are generated, you can
clean up the project directory with

    $ debuild clean

debuild creates the packages in the parent directory of the project. Install
them with

    $ cd ..
    $ sudo dpkg -i liblely-co*.deb

### configure, make, make install

It is also possible to build and install the library by hand. First, configure
the build system by running

    $ ./configure

from the root directory of the project.

The `configure` script supports many options. The full list can be shown with

    $ ./configure --help

Once the build system is configured, the library can be built with

    $ make

If the Lely TAP library ([liblely-tap]) is installed, the test suite can be run
with

    $ make check

Run

    $ make html

to generate the HTML documentation from the source code. The binaries, headers
and documentation can be installed by running

    # make install

as root.

### Options

liblely-co is written in C, but a C++ interface is provided. This interface
requires support from liblely-util. If the C++ interface is not required, or
liblely-util was built without C++ support, it can be disabled with the
`--disable-cxx` option to `configure` or by defining the `LELY_NO_CXX`
preprocessor macro.

Some of the tools provided by liblely-co use the multithreading functions of
liblely-libc. If multithreading support was disabled or unavailable when
liblely-libc was built, it should also be disabled when building liblely-co.
This can be achieved by providing the `--disable-threads` option to `configure`
or by defining the `LELY_NO_THREADS` preprocessor macro.

liblely-co uses the CAN message type provided by liblely-can, which supports
both CAN and CAN FD frames. However, if CAN FD support was disabled when
liblely-can was built, it should also be disabled when building liblely-co. This
can be achieved by providing the `--disable-canfd` option to `configure` or by
defining the `LELY_NO_CANFD` preprocessor macro.

Much of the functionality of CANopen is optional and can be disabled to save
space on embedded devices. liblely-co supports the following `configure` options
(or preprocessor macros) to disable certain features:

* `--disable-dcf` (`LELY_NO_CO_DCF`):
  disable EDS/DCF support.
* `--disable-obj-file` (`LELY_NO_CO_OBJ_FILE`):
  disable UploadFile/DownloadFile support for the object dictionary.
* `--disable-sdev` (`LELY_NO_CO_SDEV`):
  disable static device description support.
* `--disable-csdo` (`LELY_NO_CO_CSDO`):
  disable Client-SDO support (also disables master support).
* `--disable-rpdo` (`LELY_NO_CO_RPDO`):
  disable Receive-PDO support.
* `--disable-tpdo` (`LELY_NO_CO_TPDO`):
  disable Transmit-PDO support.
* `--disable-sync` (`LELY_NO_CO_SYNC`):
  disable synchronization (SYNC) object support.
* `--disable-time` (`LELY_NO_CO_TIME`):
  disable time stamp (TIME) object support.
* `--disable-emcy` (`LELY_NO_CO_EMCY`):
  disable emergency (EMCY) object support.
* `--disable-lss` (`LELY_NO_CO_LSS`):
  disable Layer Setting Services (LSS) and protocols support.
* `--disable-wtm` (`LELY_NO_CO_WTM`):
  disable Wireless Transmission Media (WTM) support.
* `--disable-master` (`LELY_NO_CO_MASTER`):
  disable master support.
* `--disable-gw` (`LELY_NO_CO_GW`):
  disable gateway support.
* `--disable-gw-txt` (`LELY_NO_CO_GW_TXT`):
  disable ASCII gateway support.

The following preprocessor macros can be defined to change the default timeouts
used by an NMT master when booting a slave:

* `LELY_CO_NMT_TIMEOUT`: the default timeout (in milliseconds) for SDO requests
  issued by an NMT master (default: 100). The actual timeout can be changed at
  runtime with co_nmt_set_timeout().
* `LELY_CO_NMT_BOOT_WAIT_TIMEOUT`: the timeout (in milliseconds) before an NMT
  master tries to boot a slave on error status B (default: 1000, see Fig. 4 in
  CiA 302-2 version 4.1.0).
* `LELY_CO_NMT_BOOT_SDO_RETRY`: the number of times an NMT master retries an SDO
  request on timeout (default: 3). This is used for SDO requests that may occur
  right after a slave receives a reset node or reset communication command,
  which might cause it to miss the request.
* `LELY_CO_NMT_BOOT_RTR_TIMEOUT`: the timeout (in milliseconds) after an NMT
  master sends a node guarding RTR to check the NMT state of a slave during
  booting (default: 100, see Fig. 9 in CiA 302-2 version 4.1.0).
* `LELY_CO_NMT_BOOT_CHECK_TIMEOUT`: the time (in milliseconds) between
  successive checks of the flash status indication (1F57:01) or program control
  (1F51:01) sub-object of a slave during booting (default: 100, see Fig. 3 in
  CiA 302-3 version 4.1.0).

Usage
-----

See [doc/overview.md](@ref md_doc_overview) for an overview of the provided
functionality. Doxygen documentation for the latest development version can be
found at http://lely_industries.gitlab.io/co/doxygen/.

Licensing
---------

Copyright 2017 [Lely Industries N.V.]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

[CiA]: http://can-cia.org/
[Cygwin]: https://www.cygwin.com/
[doxygen]: http://www.doxygen.org/
[GCC ARM Embedded]: https://launchpad.net/gcc-arm-embedded
[GitLab]: https://gitlab.com/lely_industries/co
[Graphviz]: http://www.graphviz.org/
[Lely Industries N.V.]: http://www.lely.com
[liblely-can]: https://gitlab.com/lely_industries/can
[liblely-co]: https://gitlab.com/lely_industries/co
[liblely-io]: https://gitlab.com/lely_industries/io
[liblely-libc]: https://gitlab.com/lely_industries/libc
[liblely-tap]: https://gitlab.com/lely_industries/tap
[liblely-util]: https://gitlab.com/lely_industries/util
[MinGW-w64]: http://mingw-w64.org/

