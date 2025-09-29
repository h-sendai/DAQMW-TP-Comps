# DAQ-Middleware Source - Sink スループット測定

SourceComp - SinkComp を1台の計算機で走らせて
DAQコンポーネント間のスループットを測定する。

SourceCompが1回に送るデータサイズを1 kBから4096 kBまで
変更して測定する。

## 準備

### /etc/omniORB.cfgの設定

2MB以上のデータを送信できるようにするために/etc/omniORB.cfgに
次に行を追加する。

```
# 8MB
giopMaxMsgSize = 8388608
```

### config.xml

source-sink.xml中のパラメータ
```
<params>
    <param pid="bufsize_kb">1</param>
</params>
```

でSourceCompが送るデータサイズを決める。

source-sink.xml.inからcreate-config-with-datasize.sh
で作るようにしてある。

## 走らせ方

```
./go
```

で走る。各データサイズ(1kB から 4096kBまで)3回づつ走る。
ログは``log``ディレクトリにrun.1 からrun.4096とできる。
