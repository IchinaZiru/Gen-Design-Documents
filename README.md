# 日本語版設計書自動生成手順

このリポジトリでは、Doxygen＋Graphviz＋LaTeX を組み合わせて
`任意のC++ソースコード` から高品質な PDF 設計書を自動生成する手順をまとめています。

---

## 1. 前提条件

* **OS**：Ubuntu 20.04 以上（TeX Live が導入可能な環境）
* **インストール済みツール**：

  * Doxygen (v1.8.20 以上推奨)
  * Graphviz (`dot` コマンド)
  * TeX Live

    * `texlive-full`
    * `texlive-lang-japanese`
    * `dvipdfmx`
    * platex

```bash
sudo apt update
sudo apt install doxygen graphviz texlive-full texlive-lang-japanese dvipdfmx
```

---

## 2. ディレクトリ構成例

```
generate-design-documents/
├── 対象ソース.cpp
├── Doxyfile    //Doxygen設定ファイル
├── output/     //Genarated Document Tree
├── source/     //src格納場所
└── Makefile  ← 任意
```

* `Doxyfile`：Doxygen の設定ファイル

---

## 3. Doxygen 設定のポイント

```ini
# ソース抽出設定
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = YES
EXTRACT_STATIC         = YES
EXTRACT_LOCAL_CLASSES  = YES

# 入力ファイル
INPUT                  = ./任意のソースコードファイル
FILE_PATTERNS          = *.cpp *.h

# Graphviz 図の出力
HAVE_DOT               = YES
DOT_IMAGE_FORMAT       = png
CLASS_DIAGRAMS         = YES
CALL_GRAPH             = YES
CALLER_GRAPH           = YES
DOT_GRAPH_MAX_NODES    = 100
DOT_GRAPH_DIRECTION    = TB
```

---

## 4. ドキュメント生成手順

ターミナルで以下を順に実行してください。

### 4.1 Doxygen による LaTeX 出力

```bash
# プロジェクト直下から
doxygen Doxyfile
```

* 成功すると `output/latex/` （または `latex/`）配下に `refman.tex`、`.tex` ファイル群、`.png` 画像が生成されます。

### 4.2 スタイル＆テンプレートの上書き（任意）

```bash
cd docs/latex
cp ../../doxygen.sty .
cp ../../longtable_doxygen.sty .
cp ../../refman.tex .
```

> ※前回生成時のレイアウト・設定を完全再現したい場合に実施します。

### 4.3-1 LaTeX コンパイル(pLaTeX)

```bash
# カレントディレクトリを output/latex にしてから実行
platex refman.tex
platex refman.tex
dvipdfmx refman.dvi
```

* `platex` は日本語コメント対応済み
* `dvipdfmx` により PDF 化

> **ヒント**: `\PassOptionsToPackage{dvipdfmx}{graphicx}` を `refman.tex` の先頭に追加しておくと、PNG 画像の読み込みが確実になります。

### 4.3-2 LaTeX コンパイル(LuaLaTeX)

```bash
# カレントディレクトリを output/latex にしてから実行
lualatex refman.tex
lualatex refman.tex
```

> **ヒント**: コンパイルは必ず２回行ってください(一回目のコンパイル時の.auxを２回目に読み込む必要があるため)。目次がものすごく多段や複数のカスタムスタイルの時は３回以降コンパイルする必要があります。。

---

## 5. 出力結果

* `output/latex/refman.pdf`：最終的な設計書 PDF
* 各章にクラス図・コールグラフ・インクルード図が含まれています。

---

## 6. Makefile（サンプル）

```makefile
LATEX_DIR = docs/latex

all: pdf

doxygen:
	doxygen Doxyfile

copy-styles:
	cp doxygen.sty $(LATEX_DIR)/
	cp longtable_doxygen.sty $(LATEX_DIR)/
	cp refman.tex $(LATEX_DIR)/

pdf: doxygen copy-styles
	cd $(LATEX_DIR) && \
	platex refman.tex && \
	platex refman.tex && \
	dvipdfmx refman.dvi

clean:
	rm -rf $(LATEX_DIR)/*.aux $(LATEX_DIR)/*.log \
	       $(LATEX_DIR)/*.toc $(LATEX_DIR)/*.dvi \
	       $(LATEX_DIR)/*.pdf
```

* `make pdf` で一連の処理が完了します。

---

## 7. トラブルシューティング

* **`I can’t find file 'refman.tex'`**
  → `cd docs/latex` していない可能性があります。
* **PNG PDF が読み込めない(not found.)**
  → `\PassOptionsToPackage{dvipdfmx}{graphicx}` を追加してください。(`refman.tex`の`documentclass`よりも前に追加.)
* **`非ANCII文字でLaTeXが止まる`**
  → `\usepackage[utf8]{inputenc}` を削除して下さい。
  → 上記の手段でもエラーが出る場合はLatexエンジンを`lualatex`に切り替える必要があります。
  → (`refman.tex`の`documentclass`の後に`\usepackage{luatexja}`を追加し`lualatexでコンパイル)
* **日本語文字エラー**
  → `platex + dvipdfmx` でビルドしてください。
* **! Undefined control sequence.XXXXXXX**
  → `refman.tex`の該当箇所をコメントアウトor削除してください。

* **GraphViz図が挿入されない**
* LuaLaTeXの場合 :
  → inputencは不要です。代わりにluatexjaを入れてください。また、PassOptionToPackage(dvipdfmx等)は不要です。
* pLaTeXの場合 :
  → `\PassOptionsToPackage{dvipdfmx}{graphicx}`を追加してください。
* **ハイパーリンクが機能しない**
* LuaLaTexの場合 :
  → `refman.tex`や`doxygen.sty`で`\usepackage[unicode]{hyperref}`等の`hyperref`が重複している可能性があります。該当箇所を削除orコメントアウトしてください。
* pLaTeXの場合 :
  → `refman.tex`中の`\usepackage[ps2pdf,pagebackref=true]{hyperref}`を`\usepackage[dvipdfmx,pagebackref=true]{hyperref}`に書き換えてください。
  > それでも機能しない場合(リンクはクリックできるが、飛ばない等)
  > ```bash
  > rm refman.aux refman.toc refman.out //古い .aux が残っているとラベルが混乱して飛ばなくなる。
  >platex refman.tex
  >platex refman.tex
  >dvipdfmx refman.dvi
  > ```


# LuaLaTeXサンプルスぺニット
```tex
・・・・
\documentclass[twoside]{book}

% LuaLaTeX 用
\usepackage{luatexja}
\usepackage{luatexja-fontspec}
\usepackage{fontspec}

% 必要に応じてフォント指定
\setmainjfont{IPAexMincho}
\setsansjfont{IPAexGothic}

% PNG 画像を扱う
\usepackage{graphicx}
・・・・
```

# pLaTeXサンプルスぺニット
```tex
・・・・
\PassOptionsToPackage{dvipdfmx}{graphicx}

\documentclass[twoside]{book}

% inputenc は削除する（UTF-8 なのに EUC-JP になると衝突するため）
% \usepackage[utf8]{inputenc} ← 削除！
・・・・
  % Hyperlinks
    % Hyperlinks (required, but should be loaded last)
    \ifpdf
      \usepackage[pdftex]{hyperref}
    \else
      \ifxetex
        \usepackage{hyperref}
      \else
        \usepackage[dvipdfmx]{hyperref}
      \fi
    \fi
・・・・

```
---

これで誰でも同じ手順で設計書を再現できます！
