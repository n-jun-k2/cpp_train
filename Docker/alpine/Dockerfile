FROM alpine:3.7 AS setup_stage

ENV BOOST_VERSION 1.70.0
ENV BOOST_FILE_VERSION 1_70_0
ENV GOOGLE_TEST_VERSION 1.8.0
# 開発に必要なパッケージをインストール
RUN apk update \
    && apk add --no-cache curl \
        make \
        cmake \
        g++ \
        gcc \
        ninja\
        clang \
        libstdc++ \
        llvm \
        gdb \
    && mkdir /tmp/src
# BoostとGoogle testをダウンロードし解凍する。
WORKDIR /tmp/src
RUN curl -OL https://dl.bintray.com/boostorg/release/${BOOST_VERSION}/source/boost_${BOOST_FILE_VERSION}.tar.gz \
    && tar -zxvf boost_${BOOST_FILE_VERSION}.tar.gz \
    && curl -OL https://github.com/google/googletest/archive/release-${GOOGLE_TEST_VERSION}.tar.gz \
    && tar -zxvf release-${GOOGLE_TEST_VERSION}.tar.gz \
    && mkdir ./googletest-release-${GOOGLE_TEST_VERSION}/build
# Google testのインストール
WORKDIR /tmp/src/googletest-release-${GOOGLE_TEST_VERSION}/build
RUN cmake ..\
    && make \
    && make install
# boostのインストール
WORKDIR /tmp/src/boost_${BOOST_FILE_VERSION}
RUN ./bootstrap.sh --prefix=/usr/local/ --with-libraries=program_options \
    && ./b2 install
# インストール作業で出た一時的なファイルを削除
WORKDIR /tmp
RUN rm -rf ./src/