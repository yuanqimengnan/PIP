# -*- codeing = utf-8 -*-
# @Time : 2023/9/23 20:59
# @Author : jszmwq
# @File : normal_distribution.py
# @Software: PyCharm

# 正态分布
import numpy as np
import matplotlib.pyplot as plt
import sys

# # 生成点的数量
# NY_num = 10000
# US_num = 10000
# 美国经纬度范围(30,50)(-120,-70)
# US_x = np.random.normal(loc=40, scale=3, size=US_num)
# US_y = np.random.normal(loc=-95, scale=8, size=US_num)
# plt.scatter(US_x, US_y, s=0.3, c='b')
# plt.title("US")
# plt.show()
# plt.clf()
# 纽约市经纬度范围(40.63,40.85)(-74.03,-73.75)
# NY_x = np.random.normal(loc=40.74, scale=0.03, size=NY_num)
# NY_y = np.random.normal(loc=-73.89, scale=0.05, size=NY_num)
# plt.scatter(NY_x, NY_y, s=0.3, c='g')
# plt.title("NY")
# plt.show()
# # 生成美国数据点文件
# point_file = open("US_normal_distribution.loc", "w")
# point_file.write("{}\n".format(US_num))
# for i in range(US_num):
#     point_file.write("{} {:.6f} {:.6f}\n".format(i, US_x[i], US_y[i]))
# point_file.close()
# # 生成纽约数据点文件
# point_file = open("NY_normal_distribution.loc", "w")
# point_file.write("{}\n".format(NY_num))
# for i in range(NY_num):
#     point_file.write("{} {:.6f} {:.6f}\n".format(i, NY_x[i], NY_y[i]))
# point_file.close()

# 生成点的数量
num = int(sys.argv[1])
path = sys.argv[2]
epsilon = 0.0001
selectivity_x = np.random.normal(loc=0.5, scale=0.12, size=num)
selectivity_y = np.random.normal(loc=0.5, scale=0.12, size=num)
selectivity_x = np.clip(selectivity_x, 0.0+epsilon, 1.0-epsilon)
selectivity_y = np.clip(selectivity_y, 0.0+epsilon, 1.0-epsilon)
# print(len(selectivity_y))
# plt.scatter(selectivity_x, selectivity_y, s=0.3, c='b')
# plt.title("normal_selectivity")
# plt.show()
# plt.clf()
# 生成选择率数据点文件
point_new_file = open(path, "w")
for i in range(num):
    point_new_file.write("{:.4f} {:.4f}\n".format(selectivity_x[i], selectivity_y[i]))
point_new_file.close()

#point_old_file = open("selectivity_old_normal_distribution_{num}.loc".format(num=num), "w")
#point_old_file.write("{}\n".format(num))
#for i in range(num):
#    point_old_file.write("{} {:.4f} {:.4f}\n".format(i, selectivity_x[i], selectivity_y[i]))
#point_old_file.close()
