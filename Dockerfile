FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    make \
    cmake \
    gdb \
    valgrind \
    git \
    vim \
    nano \
    curl \
    wget \
    strace \
    ltrace \
    man-db \
    manpages-dev \
    libpthread-stubs0-dev \
    python3 \
    tree \
    htop \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /devos/src /devos/include /devos/lib /devos/bin /devos/tests /devos/build

WORKDIR /devos

RUN echo 'export PS1="\[\033[1;36m\][DevOS]\[\033[0m\] \[\033[1;34m\]\w\[\033[0m\] $ "' >> /root/.bashrc && \
    echo 'clear' >> /root/.bashrc && \
    echo 'cat /devos/.welcome' >> /root/.bashrc

RUN cat > /devos/.welcome << 'WELCOME'
DevOS - Development Operating System

Current Directory: /devos

Directory Structure:
  /devos/src/         - Source code
  /devos/include/     - Header files
  /devos/lib/         - Compiled libraries
  /devos/tests/       - Test programs
  /devos/build/       - Build artifacts

WELCOME

CMD ["/bin/bash"]
