#!/bin/zsh

setopt extendedglob

do_run()
{
    local data_size
    local run_num
    data_size=$1
    run_num=$2

    run.py -l source-sink-${data_size}kB.xml
    sleep 5
    daqcom http://localhost/daqmw/scripts/ -c
    for r in {1..$run_num}; do
        echo "------> run $r"
        sleep 2
        daqcom http://localhost/daqmw/scripts/ -b $r
        sleep 10
        daqcom http://localhost/daqmw/scripts/ -e
        sleep 2
    done
    cp /tmp/daqmw/log.SinkComp log/run.${data_size}
    pkill -f Comp
}

mkdir -p log
for i in {0..12}; do
    data_size=$((2**${i}))
    echo "---> $data_size kB"
    do_run $data_size 3
done
