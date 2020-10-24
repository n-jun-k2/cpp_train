# Docker Error Q/A
Docker でのエラーまとめ

# returned a non-zero code: 100
以下のメッセージ対応
```bash
Get:7 http://ftp.jaist.ac.jp/pub/Linux/ubuntu focal/universe amd64 Packages [11.3 MB]
Get:8 http://ftp.jaist.ac.jp/pub/Linux/ubuntu focal/main amd64 Packages [1275 kB]
Reading package lists...
E: Release file for http://ftp.jaist.ac.jp/pub/Linux/ubuntu/dists/focal-updates/InRelease is not valid yet (invalid for another 1h 16min 40s). Updates for this repository will not be applied.
E: Release file for http://ftp.jaist.ac.jp/pub/Linux/ubuntu/dists/focal-backports/InRelease is not valid yet (invalid for another 1h 17min 2s). Updates for this repository will not be applied.
E: Release file for http://security.ubuntu.com/ubuntu/dists/focal-security/InRelease is not valid yet (invalid for another 5h 9min 54s). Updates for this repository will not be applied.
ERROR: Service 'dev_linux' failed to build : The command '/bin/sh -c sed -i 's@archive.ubuntu.com@ftp.jaist.ac.jp/pub/Linux@g' /etc/apt/sources.list     && apt-get update     && apt-get install --no-install-recommends -y         tzdata         build-essential         cmake         gdb         curl         openssh-server         ca-certificates     && apt-get clean     && rm -r /var/lib/apt/lists/*     && mkdir /tmp/src' returned a non-zero code: 100
```

原因としてDocker側の事故崩れらしいので,
``apt-get update``に以下のオプションを加え対応[参考](https://www.hamayanhamayan.com/entry/2020/03/14/113729)
```bash
apt-get -o Acquire::Check-Valid-Until=false -o Acquire::Check-Date=false update
```