# Github CLI

GitHub CLIの操作メモ

# プルリクエスト作成

```bash
$ gh pr create [flags]

# タイトル：sshのコンテナ対応
# マージ先：master
# 内容：コメント
$ gh pr create -t sshのコンテナ対応 -B main -b コメント
```

>flagsオプションを指定しなくても対話式で作成される。

### flagsオプション

| flags | overview |
| --- | --- |
| -B, --base string | コードをマージするブランチを指定 |
| -b, --body string | プルリクエストの内容を指定します。それ以外は対話型で設定します |
| -d, --draft | ドラフトプルリクエストを作成します |
| -t, --title string | タイトルを指定します。 それ以外は対話型で設定します |
| -w, --web |Webブラウザーを開いてプルリクエストを作成します。 |