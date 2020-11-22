# C++の開発

- [Boost.Test](https://www.boost.org/doc/libs/1_74_0/libs/test/doc/html/index.html)
- [editor config](./doc/editor_config.md)
- [ninja build](./doc/ninja.md)

#  Prerequisites
以下のソフトをインストールしてください。
- [Docker](https://docs.docker.com/get-docker/)

# How to run

開発者の環境へログインする手順
```bash
docker-compose up -d

# linux developer
docker-compose exec dev_linux /bin/bash
# create project
root@...: make project name=<new project name>
# build project
root@...: make build dir=<project name>

#Executable file
root@...: ./<project name>/build/<project name>

#Run the test
root@...: ./<project name>/build/<project name>-Test

# windows developer
・・・
```