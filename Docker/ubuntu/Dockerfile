FROM ubuntu:20.04

ARG SSH_PASSWORD

ENV BOOST_VERSION 1.70.0
ENV BOOST_FILE_VERSION 1_70_0
ENV GOOGLE_TEST_VERSION 1.8.0
ENV PYTORCH_VERSION 1.6.0
ENV DEBIAN_FRONTEND=noninteractive


# 開発に必要なパッケージをインストール(ダウンロード元のリポジトリを日本サーバーに変更)
RUN sed -i 's@archive.ubuntu.com@ftp.jaist.ac.jp/pub/Linux@g' /etc/apt/sources.list \
    && apt-get -o Acquire::Check-Valid-Until=false -o Acquire::Check-Date=false update \
    && apt-get install --no-install-recommends -y \
        tzdata \
        build-essential \
        cmake \
        gdb \
        curl \
        openssh-server \
        ca-certificates \
    && apt-get clean \
    && rm -r /var/lib/apt/lists/* \
    && mkdir /tmp/src
# sshの設定
RUN mkdir /var/run/sshd \
    && echo 'root:dammy' | chpasswd \
    && sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin prohibit-password/' /etc/ssh/sshd_config\
    && sed -i 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' /etc/pam.d/sshd
# BoostとGoogle testをダウンロードし解凍する。
WORKDIR /tmp/src
RUN curl -OL https://dl.bintray.com/boostorg/release/${BOOST_VERSION}/source/boost_${BOOST_FILE_VERSION}.tar.gz\
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
# 最後に作業場を参照
WORKDIR /tmp/project



# 以降SSH起動に必要な設定を行う。

COPY ./config/id_rsa.pub /root/.ssh/authorized_keys

ENV NOTVISIBLE="in users profile"
RUN echo "export VISIBLE=now" >> /etc/profil

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]