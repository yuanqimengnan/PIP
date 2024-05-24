#!/bin/bash
num_points=(10 100 1000 10000 100000 1000000)
for num_point in "${num_points[@]}"
do
    pointPath="/home/liuziang/Desktop/PIP/data/points/selectivity_new_normal_distribution_$num_point.loc"
    python3 normal_distribution.py "$num_point" "$pointPath"
done

for num_point in "${num_points[@]}"
do
    pointPath="/home/liuziang/Desktop/PIP/data/points/selectivity_new_uniform_distribution_$num_point.loc"
    python3 uniform_distribution.py "$num_point" "$pointPath"
done
