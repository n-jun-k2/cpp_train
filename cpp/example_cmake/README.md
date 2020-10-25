# How to build

1. build ディレクトリを作成
cmakeでは、build用の作業ディレクトリを作成しその中でビルドを行う。
```
mkdir build
cd build
```

2. Configurate & Generate

``..``はCMakeLists.txtがある、ディレクトリを指定している。相対パスの``../``に匹敵する。

```
cmake ..
```

3. build
CMakeが適当なビルドツールを選択し、ビルドを行う。
```
cmake --build .
```