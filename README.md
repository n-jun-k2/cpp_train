# C++の開発

- [Boost.Test](https://www.boost.org/doc/libs/1_74_0/libs/test/doc/html/index.html)

# コーディング規約メモ

- インデント：スペース4文字
- 必ず，半角スペース4つで統一すること．タブはエディタや環境によって幅が異なる場合がある
- 警戒レベルを最大化

    |ファイル種類 | 拡張子 |
    | --- | --- |
    |ソースファイル | file_name.cpp |
    |ヘッダファイル | header_name.hpp |

    | 命名規則名 | 用途 |
    | --- | --- |
    | PascalCase | クラス名 |
    | camelCase | メソッド名，関数名，変数名． |
    | snake_case | ファイル名．名前空間．std のメソッド名，変名，  型名に準じる場合 |
    | SNAKE_CASE | マクロ名，static const 定数，列挙体 |
