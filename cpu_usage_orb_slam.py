import psutil
import os
from multiprocessing import Process
import numpy as np
import time
import argparse
import sys

'''
path should be checked before use!
'''

number_of_cpu_usage_pts = 0
cpu_usage_total = [0] * 8
seq = 0


def parse_args():
    parser = argparse.ArgumentParser(description="orb cpu time")
    parser.add_argument("--kitti_sequence", type=int,
                        help="kitti_sequence", default=0)
    if (len(sys.argv) == 1):
        # python file opened without any argument
        parser.print_help()
        sys.exit(1)
    args = parser.parse_args()
    return args


def cpu_percent_show():
    global number_of_cpu_usage_pts, cpu_usage_total

    cpu_usage = psutil.cpu_percent(interval=0.5, percpu=True)
    cpu_usage_total = [int(cpu_usage[i]) + cpu_usage_total[i]
                       for i in range(len(cpu_usage_total))]
    number_of_cpu_usage_pts += 1
    print(cpu_usage)


def kitti_start_func():
    global seq
    seq_txt = ""
    yaml_flie_name = ""

    if (seq < 10):
        seq_txt = "0" + str(seq)
    else:
        seq_txt = str(seq)

    if (seq >= 0 and seq <= 2):
        yaml_flie_name = "KITTI00-02.yaml"
    elif (seq == 3):
        yaml_flie_name = "KITTI03.yaml"
    else:
        yaml_flie_name = "KITTI04-12.yaml"

    path_to_orb_slam = "/home/jong/github_utils/ORB_SLAM2_CV4"
    cc_file = path_to_orb_slam + "/Examples/Monocular/mono_kitti"
    voca_file = path_to_orb_slam + "/Vocabulary/ORBvoc.txt"
    yaml_file = path_to_orb_slam + "/Examples/Monocular/" + yaml_flie_name
    img_file = "~/Downloads/data_odometry_gray/dataset/sequences/" + seq_txt
    kitti_statrt_code = ' '.join([cc_file, voca_file, yaml_file, img_file])
    os.system(kitti_statrt_code)


def start():
    global number_of_cpu_usage_pts, cpu_usage_total
    p1 = Process(target=kitti_start_func)
    p2 = Process(target=cpu_percent_show)
    p1.start()
    time.sleep(10)
    print("start now!")
    while (p1.is_alive()):
        if (not p2.is_alive()):
            p2.run()

    cpu_avg_usage = [round(cpu_usage_total[i]
                     / number_of_cpu_usage_pts, 2) for i in range(len(cpu_usage_total))]
    print("\n\n===========Final CPU average usage=============")
    print(cpu_avg_usage)


if __name__ == "__main__":
    args = parse_args()
    seq = int(args.kitti_sequence)
    start()
