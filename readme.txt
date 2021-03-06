ARTK_MMD  v0.7
Copyright (C) 2009  PY

ARToolKitを使って、マーカーの上にMikuMikuDance用のモデルを表示します
音は出ません

●動作環境
    WindowsXP SP3 日本語版 (32bitのみで確認) by PY
    MacOSX 10.6(Snow Leopard) by yoneken
    Linux (Fedora14 x86_64) by yoneken

    3D表示にはOpenGLを使用しています
    グラフィックカードの機能はほとんど使わず、CPUでゴリゴリやってます

●使い方
    ・マーカーを準備します
        marker.pngを印刷するか、または手書きで作成してください(作者は手書きしています)
        手書きの場合は、marker_guide.pngを参考にしてください
        また直線はなるべく定規で引きましょう
        印刷でも手書きでも出来上がったマーカーは厚紙に貼るなどして、しわができないようにしてください
    ・PCにUSBカメラが接続されていることを確認してください
    ・ARTK_MMD.exeをダブルクリックで実行します
    ・最初にUSBカメラの設定ダイアログが出ますので[OK]ボタンを押してください
    ・USBカメラでマーカーが画面に映るよう調整します
    ・マウス右クリックでメニューを開き[Open Model(PMD)]を選びます
    ・ファイル選択ダイアログが開くので好きなモデルデータを選んでください
    ・マーカーの上にモデルが表示されたら、またメニューを開き次は[Open  Motion(VMD)]を選びます
    ・ファイル選択ダイアログが開くので好きなモーションを選んでください
    ・モデルが動き出すのでカメラを動かしたり、マーカーを移動させたり、カメラ目線モードにしたりして楽しんでください

●操作方法
    マウス右クリック ・・・ ポップアップメニューを開く
        [Open Model(PMD)]      ・・・ モデルファイル(*.pmd)を開きます
        [Open Motion(VMD)]     ・・・ モーションファイル(*.vmd)を開きます
        [Look at me! (On/Off)] ・・・ カメラ目線モードをOn/Offします

    「O」キー ・・・ モデルファイル(*.pmd)を開きます
    「M」キー ・・・ モーションファイル(*.vmd)を開きます
    「L」キー ・・・ カメラ目線モードをOn/Offします
    「F」キー ・・・ フレームレート表示をOn/Offします
    「+」キー ・・・ モデルを拡大します
    「-」キー ・・・ モデルを縮小します

    終了は右上の[X]ボタンです

●注意
    ・トゥーンシェーダーを前提として作られているモデルデータは
      このプログラムではうまく表示できません
    ・モデルと合わないモーションを読み込むと見た目がすごいことになります
       (腕IKありモデルに、腕IKなしモーションを読み込ませるなど)
    ・IKの処理がおかしいので時々ひざなどがカクンとなります

●免責
    このプログラムを使用したことによる、直接あるいは間接的損害に関しての
    一切の責任を負いかねますのでご了承下さい

●ソースコード
    このプログラムのソースコードを希望する場合は、http://ppyy.hp.infoseek.co.jp/ にてダウンロードできます

[Libraries]
	ARToolKit 2.72.1
      http://www.hitl.washington.edu/artoolkit/download/

    GLUT for Win32 version 3.7.6
      http://www.xmission.com/~nate/glut.html

    Bullet Physics SDK 2.75 RC6
      http://www.bulletphysics.com/


[Revisions]
    2009/08/26 v0.7		スフィアマップ機能に対応
    2009/08/04 v0.6		物理演算修正(低フレームレート時も少しましになったけど30fps以上推奨  これ以上の安定化は無理かも)
    2009/08/01 v0.5		フレームレートをカメラに依存しないようにしてみた(物理演算がやや安定)
    2009/07/30 v0.4		物理演算対応(フレームレートが低いと、いまいち安定しません)
    2009/07/29 v0.3		補間曲線に対応、影の表示
    2009/06/21 v0.2		カメラ目線モードを修正したけどまだおかしい
    2009/06/20 v0.1		公開

[Authors]
    PY (Main, Win32)
    Web  : http://ppyy.if.land.to/
    Mail   : py1024@gmail.com

    yoneken (port to 64-bit platforms, Mac Linux)
    Web  : http://blog.livedoor.jp/k_yon/
    Mail   : midpika@hotmail.com
 
 [License]
---------------------------------------------------------------------------------------
このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフトウェア財団
によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、希望によっては
それ以降のバージョンのうちどれか)の定める条件の下で再頒布または改変することができます。

このプログラムは有用であることを願って頒布されますが、*全くの無保証* です。
商業可能性の保証や特定の目的への適合性は、言外に示されたものも含め
全く存在しません。詳しくは同梱のGNU 一般公衆利用許諾契約書をご覧ください。
---------------------------------------------------------------------------------------
