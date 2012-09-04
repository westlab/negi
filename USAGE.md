negiはLinux上で動作するレイヤ７情報検索・抽出ソフトウェアです。

インストール手順
* postgresql ver.8.2サーバ,g++ ver.4.4と以下のライブラリおよび開発パッケージが必要です。
1. zlib 	1.2.3.4
2. libpqxx 	2.6.9
3. libpcap 	0.8
4. libboost::thread 	1.42

Debian squeezeの場合、  
`#aptitude install g++ libboost-dev libpqxx-dev zlib1g-dev libpcap-dev libboost-all-dev postgresql`  
で導入できます。

PostgreSQLのユーザとデータベースを作成します。  
`$su - postgres`  
`$createuser -P YourDBUserName`  
`$createdatabase YourDBName`  

テーブルデータを作成します。template/scripts/negi.sqlがひな型です。  
`$psql -UYourDbUserName YourDBName < template/scripts/negi.sql`


コンパイルします。  
`cd negi/`  
`$make clean`  
`$make dep`  
`$make`  
negi実行バイナリができていればコンパイル成功です.

* 検索・抽出条件の設定はpostgresqlのruleテーブルより行います。  
* 実行方法はnegi [config_file]です。  
* 設定ファイルの例はtemplate/configディレクトリ以下にあります。  
pcap.confがpcapファイルの読み込み、eth1.confがethデバイスのリアルタイムキャプチャ用設定ファイルです。

実行例  
`$./negi template/config/pcap.conf`

postgresqlのsave_streamテーブル並びにsave_resultテーブルに検索・抽出データが保存されていれば実行成功です。


連絡は石田: sin＠west.sd.keio.ac.jpまで。

