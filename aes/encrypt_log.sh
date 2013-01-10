#!/bin/bash

cpu_dir="/sys/devices/system/cpu"
set_gov="userspace"
#cpu_freq=(2000000 1900000 1800000 1700000 1600000 1500000 1400000 1300000 1200000 1100000 1000000 900000 800000 700000 600000 500000 400000 300000 200000)
cpu_freq=(1400000)
#size=(1000 10000 100000 500000)
size=(1000000)
#gpu_dir="/sys/class/misc/mali0/device/clock" 
#gpu_freq=(600 543 480 420 350 266 177)

#key_size=(128 192 256)
#file_size=(1000000)
#file_size=(1000 10000 100000 500000 1000000)

#setting file permission and governor 
for ((i=0;i<1;i++))
do
    for cpu_num in $cpu_dir/cpu?
    do  
        echo $cpu_freq
        echo ${cpu_freq[$i]} > $cpu_num/cpufreq/scaling_min_freq
        echo ${cpu_freq[$i]} > $cpu_num/cpufreq/scaling_max_freq
    done
    for((k=0;k<1;k++))
    do
        for((j=0;j<1;j++))
        do
        #        for set_key_size in $key_size
            ./aes encrypt ./dummy${size[$k]} ./output 128 odroid > ./log/log_cpu${cpu_freq[$i]}_key128_size${size[$k]}_newsec4_$j
            sleep 3 
        done
    done
done
for cpu_num in $cpu_dir/cpu?
do  
    echo $cpu_freq
    echo 1400000 > $cpu_num/cpufreq/scaling_max_freq
    echo 200000 > $cpu_num/cpufreq/scaling_min_freq
done

