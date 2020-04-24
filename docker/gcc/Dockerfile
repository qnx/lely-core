ARG __GNUC__

FROM gcc:${__GNUC__}

RUN apt-get update -q \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qy --no-install-recommends \
        cython \
        libpython-dev \
        python \
        python-setuptools \
        python-wheel \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qy --no-install-recommends \
        cython3 \
        libpython3-dev \
        python3 \
        python3-setuptools \
        python3-wheel \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update -q \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qy --no-install-recommends \
        libbluetooth-dev \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update -q \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qy --no-install-recommends \
        automake \
        libtool \
    && rm -rf /var/lib/apt/lists/*