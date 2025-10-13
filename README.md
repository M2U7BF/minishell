# 使用している主な技術
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Shell](https://img.shields.io/badge/Shell_Script-121011?style=for-the-badge&logo=gnu-bash&logoColor=white)


# プロジェクトの概要
* bashコマンドの簡易実装

# 機能
* 大抵のコマンドが実行可能
* 組み込みコマンドが実行可能
  * echo （オプション -n 付き）
  * cd （相対パスまたは絶対パスのみ）
  * pwd （オプションなし）
  * export （オプションなし）
  * unset （オプションなし）
  * env （オプションまたは引数なし）
  * exit （オプションなし）
* ダブルクォーテーション（"）、シングルクォーテーション（'）に対応
* リダイレクト（>,>>,<）、パイプ（|）、ヒアドキュメント（<<）、に対応
* $?に対応
* シグナル（SIGINT）に対応

# 実行方法
1)当リポジトリをクローン。
```
git clone git@github.com:M2U7BF/minishell.git
```

2)リポジトリ直下に移動し、make実行。
```
cd minishell
make
```

# ディレクトリ構成
```
lib // ライブラリ
src // ソースコード
include // ヘッダファイル
```

# 実行例
・echoを実行
![image](https://github.com/M2U7BF/minishell/blob/main/readme/Screenshot%20from%202025-10-13%2010-41-06.png)

・pwd, cd, exitを実行
![image](https://github.com/M2U7BF/minishell/blob/main/readme/Screenshot%20from%202025-10-13%2010-43-25.png)
