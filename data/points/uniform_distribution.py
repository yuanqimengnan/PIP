# -*- codeing = utf-8 -*-
# @Time : 2023/9/23 22:34
# @Author : jszmwq
# @File : uniform_distribution.py.py
# @Software: PyCharm

# 均匀分布
import numpy as np
import matplotlib.pyplot as plt
import sys

# 生成点的数量
# NY_num = 10000000
# US_num = 10000000
# # 美国经纬度范围(30,50)(-120,-70)
# US_x = np.random.uniform(low=30, high=50, size=US_num)
# US_y = np.random.uniform(low=-120, high=-70, size=US_num)
# plt.scatter(US_x, US_y, s=0.3, c='b')
# plt.title("US")
# plt.show()
# plt.clf()
# # 纽约市经纬度范围(40.63,40.85)(-74.03,-73.75)
# NY_x = np.random.uniform(low=40.63, high=40.85, size=NY_num)
# NY_y = np.random.uniform(low=-74.03, high=-73.75, size=NY_num)
# plt.scatter(NY_x, NY_y, s=0.3, c='g')
# plt.title("NY")
# plt.show()
# 生成美国数据点文件
# point_file = open("US_uniform_distribution.loc", "w")
# point_file.write("{}\n".format(US_num))
# for i in range(US_num):
#     point_file.write("{} {:.6f} {:.6f}\n".format(i, US_x[i], US_y[i]))
# point_file.close()
# # 生成纽约数据点文件
# point_file = open("NY_uniform_distribution.loc", "w")
# point_file.write("{}\n".format(NY_num))
# for i in range(NY_num):
#     point_file.write("{} {:.6f} {:.6f}\n".format(i, NY_x[i], NY_y[i]))
# point_file.close()


# 生成点的数量
num = int(sys.argv[1])
path = sys.argv[2]
epsilon = 0.0001
selectivity_x = np.random.uniform(low=0+epsilon, high=1-epsilon, size=num)
selectivity_y = np.random.uniform(low=0+epsilon, high=1-epsilon, size=num)
# plt.scatter(selectivity_x, selectivity_y, s=0.3, c='b')
# plt.title("uniform_selectivity")
# plt.show()
# plt.clf()
# 生成选择率数据点文件
point_file = open(path, "w")
# point_file.write("{}\n".format(num))
for i in range(num):
    # point_file.write("{} {:.4f} {:.4f}\n".format(i, selectivity_x[i], selectivity_y[i]))
    point_file.write("{:.4f} {:.4f}\n".format(selectivity_x[i], selectivity_y[i]))
point_file.close()
