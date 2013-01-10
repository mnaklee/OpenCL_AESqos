#!/bin/bash

cpu_dir="/sys/devices/system/cpu"
set_gov="userspace"
#cpu_freq=(2000000 1800000 1600000 1400000 1200000 1000000 800000 600000 400000 200000)
#cpu_freq=(2000000 1900000 1800000 1700000 1600000 1500000 1400000 1300000 1200000 1100000 1000000 900000 800000 700000 600000 500000 400000 300000 200000)
cpu_freq=(1400000)

gpu_dir="/sys/class/misc/mali0/device/clock" 
#gpu_freq=(600 543 480 420 350 266 177)
gpu_freq=(600)

key_size=(128 192 256)
file_size=(1000000)
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

    for ((j=0;j<1;j++))
    do
        echo ${gpu_freq[$j]}
        echo $gpu_dir
        echo ${gpu_freq[$j]} > $gpu_dir

#        for set_key_size in $key_size
        for((l=0;l<1;l++))
        do
            for((k=0;k<5;k++))
            do
               ./paes -i ./data/samples.txt -o ./output/output -m encrypt -d gpu -k 128 -s ${file_size[$l]} -p odroid > ./log_wb/log_wb_cpu${cpu_freq[$i]}"_gpu"${gpu_freq[$j]}"_key128_size"${file_size[$l]}"_"$k".txt"
               echo "./paes -i ./data/samples.txt -o ./output/output -m encrypt -d gpu -k 128 -p odroid > ./log/log_cpu${cpu_freq[$i]}"_gpu"${gpu_freq[$j]}"_key128_size"${file_size[$l]}"
               sleep 5
            done
        done
    done
done

for cpu_num in $cpu_dir/cpu?
do  
    echo $cpu_freq
    echo 1400000 > $cpu_num/cpufreq/scaling_max_freq
    echo 200000 > $cpu_num/cpufreq/scaling_min_freq
done

