# エディターコンフィグ

[参考](https://editorconfig.org/)

## ファイルパターン

| パターン | 説明 |
| --- | --- |
| `[*]` |全てのファイル |
| `[*.md]` | 特定の拡張子のファイル |
| `[*.{less,css,scala}]` |複数の拡張子のファイル |
| `[Makefile] `| 特定のファイル名のファイル |

## 設定項目
| 項目 | 内容 | コード |
| --- | --- | --- |
| charset | 文字コード | latin1, utf-8, utf-8-bom, utf-16be, utf-16le |
| end_of_line | 改行コード | cr, lf, crlf |
| insert_final_newline | ファイル末尾に空行を挿入するか | true |
| indent_size | インデントのサイズ | 2, 4, 8 |
| indent_style | インデントはタブかスペースか | space, tab |
| trim_trailing_whitespace | 行末尾のホワイトスペース（空白文字）を削除するかどうか | true, false |
