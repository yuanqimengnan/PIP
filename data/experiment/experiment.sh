#!/bin/bash
experimentPath="/home/liuziang/Desktop/PIP/data/experiment/experiment.csv"
if [ ! -f "$experimentPath" ]; then
    > "$experimentPath"
    echo "File created: $experimentPath"
else
    echo "File already exists: $experimentPath"
fi

current_time=$(date "+%Y-%m-%d %H:%M:%S")
echo "Current Time : $current_time" >> $experimentPath

num_points=(10 100 1000 10000 100000 1000000)
polygonPath="/home/liuziang/Desktop/PIP/data/polygons/new_plan_polygon.txt"

# normal
cd /home/liuziang/Desktop/PIP/
echo "GPUPIP_normal" >> $experimentPath
make
for num_point in "${num_points[@]}"
do
    pointPath="/home/liuziang/Desktop/PIP/data/points/selectivity_new_normal_distribution_$num_point.loc"
    parameter="--polyPath $polygonPath --pointPath $pointPath --outputResult --experimentPath $experimentPath --nIter 10"
    ./PIP $parameter
done

cd /home/liuziang/Desktop/PIP/data/experiment
echo "RayCasting_normal" >> $experimentPath
g++ -fopenmp RayCasting.cpp -o RayCasting
for num_point in "${num_points[@]}"
do
    pointPath="/home/liuziang/Desktop/PIP/data/points/selectivity_new_normal_distribution_$num_point.loc"
    parameter="$pointPath $polygonPath $experimentPath"
    ./RayCasting $parameter
done

cd /home/liuziang/Desktop/PIP/data/experiment
echo "PostGIS_normal" >> $experimentPath
parameter="prepare $polygonPath"
python3 Postgis.py $parameter

for num_point in "${num_points[@]}"
do
    pointPath="/home/liuziang/Desktop/PIP/data/points/selectivity_new_normal_distribution_$num_point.loc"
    parameter="query $polygonPath $pointPath $experimentPath"
    python3 Postgis.py $parameter
done

# uniform
cd /home/liuziang/Desktop/PIP/
echo "GPUPIP_uniform" >> $experimentPath
make
for num_point in "${num_points[@]}"
do
    pointPath="/home/liuziang/Desktop/PIP/data/points/selectivity_new_uniform_distribution_$num_point.loc"
    parameter="--polyPath $polygonPath --pointPath $pointPath --outputResult --experimentPath $experimentPath --nIter 10"
    ./PIP $parameter
done

cd /home/liuziang/Desktop/PIP/data/experiment
echo "RayCasting_uniform" >> $experimentPath
g++ -fopenmp RayCasting.cpp -o RayCasting
for num_point in "${num_points[@]}"
do
    pointPath="/home/liuziang/Desktop/PIP/data/points/selectivity_new_uniform_distribution_$num_point.loc"
    parameter="$pointPath $polygonPath $experimentPath"
    ./RayCasting $parameter
done

cd /home/liuziang/Desktop/PIP/data/experiment
echo "PostGIS_uniform" >> $experimentPath
parameter="prepare $polygonPath"
python3 Postgis.py $parameter

for num_point in "${num_points[@]}"
do
    pointPath="/home/liuziang/Desktop/PIP/data/points/selectivity_new_uniform_distribution_$num_point.loc"
    parameter="query $polygonPath $pointPath $experimentPath"
    python3 Postgis.py $parameter
done
