FROM ubuntu:22.04

ARG DEVCONTAINER
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -qq \
    && apt-get install -y \
      # x86_64 / generic packages
      bash \
      build-essential \
      cmake \
      git \
      make \
      python3 \
      python3-pip \
      python-is-python3 \
      tar \
      unzip \
      wget \
      curl \
      # aarch64 packages
      libffi-dev \
      libssl-dev \
      python3-dev \
      git \
      apt-utils \
      pkg-config \
      libpixman-1-dev \
      libcairo2-dev \
      libpango-1.0-0 \
      ibpango1.0-dev \
      libpangocairo-1.0-0 \
      libncurses5 \
      sudo \
      openocd \
    && curl -sL https://deb.nodesource.com/setup_18.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/cache/apt/* /var/lib/apt/lists/*;

# Devcontainer specific
RUN apt-get update -qq \
    && apt-get install -y \
        clang-format-14 \
        clang-tidy

# Git needed for PROJECT_GIT_COMMIT_HASH variable setting

RUN pip3 install adafruit-nrfutil
RUN pip3 install -Iv cryptography==3.3
RUN pip3 install cbor
RUN npm i lv_font_conv@1.5.2 -g

RUN npm i ts-node@10.9.1 -g
RUN npm i @swc/core -g
RUN npm i lv_img_conv@0.3.0 -g

# build.sh knows how to compile
COPY build.sh /opt/

RUN bash -c "source /opt/build.sh; GetGcc;"
RUN bash -c "source /opt/build.sh; GetNrfSdk;"
RUN bash -c "source /opt/build.sh; GetMcuBoot;"

RUN adduser infinitime
RUN adduser infinitime sudo

ENV SOURCES_DIR /sources
CMD ["/opt/build.sh"]
