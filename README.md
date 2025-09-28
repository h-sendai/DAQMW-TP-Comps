# DAQ-Middleware DAQコンポーネント間レイテンシの測定

## ディレクトリ構成

```
/home/daq/DAQMW-Latency/LatencySender
/home/daq/DAQMW-Latency/LatencyLogger
/home/daq/DAQMW-Latency/latency-sender-logger.xml
```

## /etc/omniORB.cfgの設定

2MB以上のデータを送信するために/etc/omniORB.cfgに次の行を
追加する。

```
/etc/omniORB.cfg
# 8MB
giopMaxMsgSize = 8388608
```

## config.xml

latency-sender-logger.xml中のパラメータ

```
<params>
    <param pid="bufsize_kb">128</param>
</params>
```

でLatencySenderCompが送るデータサイズを決める。

## LatencySender

bufsize_kb*1024のデータを後段LatencyLoggerに送る。
送るデータのデータフォーマットは以下のとおり:

```
63                        0
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
```

struct timespecは送る直前にclock_gettime()で時刻情報を
埋め込む。

## LatencyLogger

LatencyLoggerでは受け取ったデータの24バイト名（0オリジン)
に受け取った時刻をstruct timespecで埋める。
さらにdaq_run()にはいる直前にInPortバッファでまだ
読まれていないバッファ個数を追加する。
このバッファ個数を取得できるようにするためには/usr/include/rtm/InPort.hを
InPort.getUsedBufferLen.hと入れ替える必要がある。

```
63                        0
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| number of unread buf    | (8バイト)
+-------------------------+
```

合計40バイトをディスクに記録する。
記録されるデータは次のようになる:

```
63                        0
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| number of unread buf    | (8バイト)
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| number of unread buf    | (8バイト)
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| number of unread buf    | (8バイト)
+-------------------------+
(以下送った回数分続く)
```

## 走らせ方

```
./go
logディレクトリにデータがコピーされている
cd log
../data2log.zsh
../result.zsh
```
最後のresult.zshで
```
バッファサイズ(kB) レイテンシの平均値(us) 標準偏差(us)
```
ができる。
```

グラフを作るにはplot.shを書き換えて作る (gnuplotが必要)。
