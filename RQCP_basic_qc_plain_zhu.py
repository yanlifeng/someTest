# 并列柱状图
import matplotlib.pyplot as plt

import numpy as np


bar_width = 0.1

plt.figure(figsize=(20, 10))#6，8分别对应宽和高




plt.subplot(2,3,1)
x = np.arange(2)
RQCP512 = [532, 1331]
RQCP256 = [577, 1465]
RQCPauto = [735, 3059]
RabbitQC = [1345, 7225]
fastp = [1109, 5670]
SOAPnuke = [2057, 4298]
Trimmomatic = [1833, 2508]
FASTQC = [2244, 0]
tick_label = ['Single-end', 'Pair-end']
plt.bar(x + bar_width * 0, RQCP512, bar_width, label='RQCP512')
plt.bar(x + bar_width * 1, RQCP256, bar_width, label='RQCP256')
plt.bar(x + bar_width * 2, RQCPauto, bar_width, label='RQCPauto')
plt.bar(x + bar_width * 3, RabbitQC, bar_width, label='RabbitQC')
plt.bar(x + bar_width * 4, fastp, bar_width, label='fastp')
plt.bar(x + bar_width * 5, SOAPnuke, bar_width, label='SOAPnuke')
plt.bar(x + bar_width * 6, Trimmomatic, bar_width, label='Trimmomatic')
plt.bar(x + bar_width * 7, FASTQC, bar_width, label='FASTQC')
plt.ylabel("runtime (s)")
plt.xticks(x + bar_width * 3, tick_label)  # 显示x坐标轴的标签,即tick_label,调整位置，使其落在两个直方图中间位置
plt.title("SRR7963242 on a 48-core Intel CPU (single thread)")
plt.legend()  # 显示图例，即label




plt.subplot(2,3,4)
x = np.arange(2)
RQCP512 = [110, 213]
RQCP256 = [115, 225]
RQCPauto = [118, 275]
RabbitQC = [132, 320]
fastp = [516, 518]
SOAPnuke = [220, 498]
Trimmomatic = [349, 367]
FASTQC = [0, 0]
tick_label = ['Single-end', 'Pair-end']
plt.bar(x + bar_width * 0, RQCP512, bar_width, label='RQCP512')
plt.bar(x + bar_width * 1, RQCP256, bar_width, label='RQCP256')
plt.bar(x + bar_width * 2, RQCPauto, bar_width, label='RQCPauto')
plt.bar(x + bar_width * 3, RabbitQC, bar_width, label='RabbitQC')
plt.bar(x + bar_width * 4, fastp, bar_width, label='fastp')
plt.bar(x + bar_width * 5, SOAPnuke, bar_width, label='SOAPnuke')
plt.bar(x + bar_width * 6, Trimmomatic, bar_width, label='Trimmomatic')
plt.bar(x + bar_width * 7, FASTQC, bar_width, label='FASTQC')
plt.ylabel("runtime (s)")
plt.xticks(x + bar_width * 3, tick_label)  # 显示x坐标轴的标签,即tick_label,调整位置，使其落在两个直方图中间位置
plt.title("SRR7963242 on a 48-core Intel CPU (multiple thread)")





plt.subplot(2,3,2)
x = np.arange(2)
RQCP512 = [0, 0]
RQCP256 = [495, 1289]
RQCPauto = [556, 2540]
RabbitQC = [1052, 4786]
fastp = [846, 4214]
SOAPnuke = [2071, 4178]
Trimmomatic = [1196, 1691]
FASTQC = [2109, 0]
tick_label = ['Single-end', 'Pair-end']
plt.bar(x + bar_width * 0, RQCP512, bar_width, label='RQCP512')
plt.bar(x + bar_width * 1, RQCP256, bar_width, label='RQCP256')
plt.bar(x + bar_width * 2, RQCPauto, bar_width, label='RQCPauto')
plt.bar(x + bar_width * 3, RabbitQC, bar_width, label='RabbitQC')
plt.bar(x + bar_width * 4, fastp, bar_width, label='fastp')
plt.bar(x + bar_width * 5, SOAPnuke, bar_width, label='SOAPnuke')
plt.bar(x + bar_width * 6, Trimmomatic, bar_width, label='Trimmomatic')
plt.bar(x + bar_width * 7, FASTQC, bar_width, label='FASTQC')
plt.ylabel("runtime (s)")
plt.xticks(x + bar_width * 3, tick_label)  # 显示x坐标轴的标签,即tick_label,调整位置，使其落在两个直方图中间位置
plt.title("SRR7963242 on a 128-core AMD CPU (single thread)")





plt.subplot(2,3,5)
x = np.arange(2)
RQCP512 = [0, 0]
RQCP256 = [123, 218]
RQCPauto = [129, 235]
RabbitQC = [135, 287]
fastp = [669, 688]
SOAPnuke = [193, 437]
Trimmomatic = [223, 287]
FASTQC = [0, 0]
tick_label = ['Single-end', 'Pair-end']
plt.bar(x + bar_width * 0, RQCP512, bar_width, label='RQCP512')
plt.bar(x + bar_width * 1, RQCP256, bar_width, label='RQCP256')
plt.bar(x + bar_width * 2, RQCPauto, bar_width, label='RQCPauto')
plt.bar(x + bar_width * 3, RabbitQC, bar_width, label='RabbitQC')
plt.bar(x + bar_width * 4, fastp, bar_width, label='fastp')
plt.bar(x + bar_width * 5, SOAPnuke, bar_width, label='SOAPnuke')
plt.bar(x + bar_width * 6, Trimmomatic, bar_width, label='Trimmomatic')
plt.bar(x + bar_width * 7, FASTQC, bar_width, label='FASTQC')
plt.ylabel("runtime (s)")
plt.xticks(x + bar_width * 3, tick_label)  # 显示x坐标轴的标签,即tick_label,调整位置，使其落在两个直方图中间位置
plt.title("SRR7963242 on a 128-core AMD CPU (multiple thread)")





plt.subplot(2,3,3)
x = np.arange(2)
RQCP512 = [51, 96]
RQCP256 = [66, 106]
RQCPauto = [104, 190]
RabbitQC = [156, 334]
fastp = [146, 291]
SOAPnuke = [0, 0]
Trimmomatic = [0, 0]
FASTQC = [0, 0]
tick_label = ['Single-end', 'Pair-end']
plt.bar(x + bar_width * 0, RQCP512, bar_width, label='RQCP512')
plt.bar(x + bar_width * 1, RQCP256, bar_width, label='RQCP256')
plt.bar(x + bar_width * 2, RQCPauto, bar_width, label='RQCPauto')
plt.bar(x + bar_width * 3, RabbitQC, bar_width, label='RabbitQC')
plt.bar(x + bar_width * 4, fastp, bar_width, label='fastp')
plt.bar(x + bar_width * 5, SOAPnuke, bar_width, label='SOAPnuke')
plt.bar(x + bar_width * 6, Trimmomatic, bar_width, label='Trimmomatic')
plt.bar(x + bar_width * 7, FASTQC, bar_width, label='FASTQC')
plt.ylabel("runtime (s)")
plt.xticks(x + bar_width * 3, tick_label)  # 显示x坐标轴的标签,即tick_label,调整位置，使其落在两个直方图中间位置
plt.title("SRR7963242 on a 4-core Intel CPU  (single thread)")





plt.subplot(2,3,6)
x = np.arange(2)
RQCP512 = [14, 29]
RQCP256 = [14, 30]
RQCPauto = [29, 53]
RabbitQC = [43, 89]
fastp = [49, 94]
SOAPnuke = [53, 107]
Trimmomatic = [46, 90]
FASTQC = [0, 0]
tick_label = ['Single-end', 'Pair-end']
plt.bar(x + bar_width * 0, RQCP512, bar_width, label='RQCP512')
plt.bar(x + bar_width * 1, RQCP256, bar_width, label='RQCP256')
plt.bar(x + bar_width * 2, RQCPauto, bar_width, label='RQCPauto')
plt.bar(x + bar_width * 3, RabbitQC, bar_width, label='RabbitQC')
plt.bar(x + bar_width * 4, fastp, bar_width, label='fastp')
plt.bar(x + bar_width * 5, SOAPnuke, bar_width, label='SOAPnuke')
plt.bar(x + bar_width * 6, Trimmomatic, bar_width, label='Trimmomatic')
plt.bar(x + bar_width * 7, FASTQC, bar_width, label='FASTQC')
plt.ylabel("runtime (s)")
plt.xticks(x + bar_width * 3, tick_label)  # 显示x坐标轴的标签,即tick_label,调整位置，使其落在两个直方图中间位置
plt.title("SRR7963242 on a 4-core Intel CPU (multiple thread)")













plt.show()
