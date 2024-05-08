ARG __GNUC__

FROM gcc:${__GNUC__}

# support for Debian 8 (GCC <= 6)
RUN VERSION=$(sed 's/\..*//' /etc/debian_version) \
    && if [ "$VERSION" -eq "8" ] ; then \
          echo "deb http://archive.debian.org/debian/ jessie main non-free contrib" > /etc/apt/sources.list ; \
          echo " deb-src http://archive.debian.org/debian/ jessie main non-free contrib" >> /etc/apt/sources.list ; \
          echo "deb http://archive.debian.org/debian-security/ jessie/updates main non-free contrib" >> /etc/apt/sources.list ; \
          echo " deb-src http://archive.debian.org/debian-security/ jessie/updates main non-free contrib" >> /etc/apt/sources.list ; \
          apt-key update -q ; \
       fi

RUN apt-get update -q \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qy --no-install-recommends \
        cython3 \
        libpython3-dev \
        python3 \
        python3-empy \
        python3-setuptools \
        python3-wheel \
        python3-yaml \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update -q \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qy --no-install-recommends \
        libbluetooth-dev \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update -q \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qy --no-install-recommends \
        automake \
        libtool \
        pkg-config \
    && rm -rf /var/lib/apt/lists/*

RUN git clone --branch v4.0 --depth 1 https://github.com/cpputest/cpputest.git \
    && cd cpputest/cpputest_build \
    && autoreconf .. -i \
    && ../configure --disable-memory-leak-detection \
    && make \
    && make install \
    && cd - \
    && rm -rf cpputest
