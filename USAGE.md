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

Centos 6.3の場合
yum install gcc-c++ libpcap-devel zlib-devel

yum install http://yum.postgresql.org/8.4/redhat/rhel-6-x86_64/libpqxx-3.1-0.1.rhel6.x86_64.rpm http://yum.postgresql.org/8.4/redhat/rhel-6-x86_64/libpqxx-devel-3.1-0.1.rhel6.x86_64.rpm http://yum.postgresql.org/8.4/redhat/rhel-6-x86_64/postgresql84-devel-8.4.13-1PGDG.rhel6.x86_64.rpm http://yum.postgresql.org/8.4/redhat/rhel-6-x86_64/postgresql84-8.4.13-1PGDG.rhel6.x86_64.rpm http://yum.postgresql.org/8.4/redhat/rhel-6-x86_64/postgresql84-libs-8.4.13-1PGDG.rhel6.i686.rpm http://yum.postgresql.org/8.4/redhat/rhel-6-x86_64/postgresql84-server-8.4.13-1PGDG.rhel6.x86_64.rpm
で導入できます。



PostgreSQLのユーザとデータベースを作成します。  
`$su - postgres`  
`$psql`  
`psql# create user YourDBUserName`  
`psql# create database YourDBName`  
`psql# grant all privileges on database YourDBName to YourDBUserName`  
`psql# \q`  
`$exit`   

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

