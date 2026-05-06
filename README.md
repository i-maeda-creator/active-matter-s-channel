# Active Matter S-Channel Simulation Archive

大学時代のアクティブマター研究コードを、再開できる形で保管するためのリポジトリです。
元フォルダ: `C:\Users\kyomi\OneDrive - cc.kyoto-su.ac.jp\2022 斎藤研\C言語`

## 何の研究に見えるか

このコードは、2次元の自己駆動粒子を S 字型の狭いチャネル内で動かすシミュレーションです。粒子は WCA 型の斥力で重なりを避け、向き `th` に沿って能動力 `FA` で進み、並進・回転ノイズを受けます。粒子間の向き相互作用として Kuramoto 型の項も実装されています。

中心になっている実行ファイルは `activematter.C` です。`zyugyo.C` は授業または基礎練習の弾性粒子シミュレーションから発展していった途中版に見えます。

## 主な構成

- `activematter.C`: S 字チャネル内のアクティブ粒子シミュレーション本体
- `zyugyo.C`: 初期の粒子シミュレーション練習コード
- `include/system.h`: 粒子数、ステップ数、系のサイズ、力学パラメータ、配列の定義
- `include/initia.h`: 初期配置、半径、角度、力の初期化
- `include/kurfor.h`: WCA 粒子間斥力と Kuramoto 型の角度相互作用
- `include/aforce.h`: 自己駆動力
- `include/rforce.h`: 並進・回転ノイズ
- `include/integrl.h`: Euler 型の時間発展、過減衰 Langevin 更新
- `include/shpfor.h`: S 字壁との WCA 相互作用とチャネル方向への角度設定
- `include/bdcond.h`: 周期境界、S 字チャネル用の接続境界
- `include/output.h`: gnuplot 用の位置・半径出力
- `anime.plt`, `out.plt`: `dat/out.dat` から GIF を作る gnuplot スクリプト
- `*.docx`, `0408.txt`: 当時の学習・研究メモ

## 現在の到達点

実装済みと思われるもの:

- N=250 の円形粒子を扱う配列ベースの C/C++ コード
- S 字型チャネルの幾何と壁斥力
- WCA 型の粒子間斥力
- 自己駆動力 `FA`
- 並進ノイズ `DD` と回転ノイズ `DR`
- Kuramoto 型の向き相互作用 `KR * sin(th[j] - th[i])`
- gnuplot による粒子位置のアニメーション出力

2026-05-06 時点の構文確認:

- `g++ -fsyntax-only activematter.C`: 成功
- `g++ -fsyntax-only zyugyo.C`: 失敗。`include/eforce.h` と `include/vforce.h` が `L` を参照する一方、現在の `include/system.h` では `LX`, `LY` が使われているため。

## 実行方法

MinGW などで `g++` が使える環境なら、まず構文確認できます。

```sh
g++ -fsyntax-only activematter.C
```

実行ファイルを作る場合:

```sh
g++ -O2 -Wall -Wextra activematter.C -o active-matter
./active-matter runs/baseline-2d
```

短い動作確認だけを行う場合:

```sh
./active-matter runs/smoke-2d-baseline 1 10 random 0.5 250 200 80
```

引数は `出力フォルダ 出力ステップ数 内側ステップ数 初期配置 半径 粒子数 LX LY` です。初期配置は `shape` または `random` です。省略すると `runs/latest 200 2000 shape 0.5 250 200 80` で実行します。
`OSTEP` と `ISTEP` はデフォルト値として記録されますが、引数でそれより大きい値も指定できます。

昔の出力に近い半径 `R=0.4` で S 字入口配置を試す場合:

```sh
./active-matter runs/shape-r04-smoke 1 10 shape 0.4 10 200 80
```

ルート直下の古い `out.dat` に近い座標範囲を試す場合:

```sh
./active-matter runs/legacy-shape-r04 1 1000 shape 0.4 10 68 35
```

現時点で最も昔の1フレーム目に近い候補:

```sh
./active-matter runs/legacy-shape-r04-ly33-step7000 1 7000 shape 0.4 10 68 33
```

`make` が使える環境では以下でも同じです。

```sh
make check
make run
```

出力:

- `runs/<run-name>/out.dat`: 各出力ステップの粒子位置 `x y r`
- `runs/<run-name>/ene.dat`: `time kin pot wpo ene`
- `runs/<run-name>/params.txt`: 実行時の主要パラメータ

古い出力と復元実験を比較する場合:

```sh
python analysis/compare_runs.py out.dat runs/legacy-shape-r04-ly34/out.dat --frames 1
python analysis/plot_frames.py out.dat runs/legacy-shape-r04-ly33-step7000/out.dat --frame 0 --output analysis/legacy_frame0_compare.png --xlim 0,34 --ylim 29,35
```

未完成または要確認のもの:

- `KR = 0.0` なので、現状設定では Kuramoto 相互作用が無効です。
- `ene.dat` は運動エネルギー計算がコメントアウトされているため、現状の `kin` は常に 0 です。
- 現在の `shape` 初期配置は、`NP=250`, `R=0.5`, `WS=3` の狭い入口に粒子を置こうとするため、配置探索に非常に時間がかかる可能性があります。昔の `out.dat` の最初のフレームは 10 粒子、半径 `0.4` で出力されており、全体の座標範囲はおよそ `x=0..68`, `y=0..34` です。当時は `NP=10`, `R=0.4`, `LX=68`, `LY=35` に近い条件だった可能性があります。
- `shpfor.h` 内でカーブ領域の `th[i]` をチャネル接線方向に直接上書きしており、粒子自身の向きダイナミクスと「壁による方向拘束」が混ざっています。
- `zyugyo.C` 側の `eforce.h` / `vforce.h` は `L` を参照しますが、現在の `system.h` には `L` がなく `LX`, `LY` になっています。過去版の名残の可能性があります。
- `dat/out.dat` と `dat/ene.dat` は 2025 年時点で空ファイルです。ルート直下の `out.dat`, `ene.dat` は 2023 年 1 月の過去出力です。

## include フォルダの読み解き

`activematter.C` が直接使う最終版に近いヘッダ:

- `system.h`: 主要パラメータとグローバル配列。`N=250`, `OSTEP=200`, `ISTEP=2000`, `LX=200`, `LY=80`。
- `initia.h`: 粒子半径、格子/ランダム/S 字チャネル入口配置、角度初期化、力の初期化。
- `kurfor.h`: 粒子間 WCA 斥力と Kuramoto 型トルク。S 字接続境界に合わせ、x 方向をまたぐと y をずらす処理が入っている。
- `shpfor.h`: S 字チャネル壁の WCA 斥力。カーブ部では粒子角度 `th` をチャネル接線方向に上書きする処理が入っている。
- `aforce.h`: `FA * (cos(th), sin(th))` の自己駆動力。
- `rforce.h`: 並進ノイズ `DD` と回転ノイズ `DR`。
- `integrl.h`: 授業版の速度あり Euler 更新 `seuler` と、アクティブマター版の過減衰更新 `ovdamp`。
- `bdcond.h`: 矩形周期境界 `period` と、S 字チャネル接続用の `pershp`。
- `output.h`: 各時刻の `x y r` を空行区切りで出力し、gnuplot の `index` でアニメーション化できる形式。

途中版・部品版に見えるヘッダ:

- `wcafor.h`: 通常の矩形周期境界での粒子間 WCA 斥力。
- `wallfor.h`: 上下直線壁の WCA 斥力。
- `shpfor2.h`: S 字壁の初期実装。現行 `shpfor.h` より前の版で、壁ポテンシャル `wpo` や角度上書きがまだない。
- `kurfor_2D.h`: Kuramoto 項を書き足そうとしている途中版。`dmt = KR*sin(th[j]-)` で途切れており、コンパイル不能。
- `genten.h`: 原点や壁に関する古い試作コードに見える。
- `eforce.h`, `vforce.h`, `bforce.h`: 授業版の弾性力・粘性力・背景減衰。現在の `system.h` とは一部パラメータ名が合っていない。

## 研究を再開するなら

短期ゴールは「S 字チャネル内で、自己駆動粒子集団がどの条件で流れ・渋滞・同期・偏りを示すかを再現可能に測る」ことが良さそうです。

長期ゴールは、アクティブマターを自己組織化する交通・都市システムの基礎モデルとして育てることです。詳しくは `docs/research-roadmap.md` にまとめています。

実験の復元ログは `docs/experiment-log.md` に残しています。

最初の再開ステップ:

1. 現在コードをそのままビルドできる状態に固定する。
2. `time` 初期化、出力先、エネルギー/観測量の整理だけを最小修正する。
3. パラメータ `FA`, `KR`, `DD`, `DR`, `N`, `WS` をコマンドラインまたは設定ファイルから変えられるようにする。
4. 観測量を追加する: 平均速度、通過流量、局所密度、向き秩序パラメータ、詰まり時間。
5. まず `KR=0` と `KR>0` の比較を行い、同期が S 字チャネル通過に与える影響を見る。
6. 再現可能な図を作る: アニメーション、流量 vs 活性、流量 vs 同期強度、密度ヒートマップ。

## 仮の研究テーマ案

「S 字型狭窄チャネルにおける自己駆動粒子集団の同期と輸送効率」

問いの形にすると:

- 自己駆動力 `FA` が大きいほど通過流量は増えるのか、それとも渋滞が増えて低下するのか。
- 向きの同期強度 `KR` は、曲がり角の通過を助けるのか、集団的な詰まりを強めるのか。
- ノイズ `DR` は、詰まりを解消する揺らぎとして働くのか、秩序を壊して輸送を落とすのか。

## 次にやる実装候補

- `src/` と `include/` に整理し、Makefile または CMake を追加する。
- 出力を `dat/` に統一し、実行ごとのパラメータを一緒に保存する。
- `analysis/` に Python か gnuplot の解析スクリプトを置く。
- 既存の `anime.gif` を再生成できるコマンドを README に固定する。
