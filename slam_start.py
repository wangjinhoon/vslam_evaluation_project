import os
import numpy as np
import time
import argparse
import sys

'''
ORB SLAM monocular KITTI easy starter for LINUX
PATH SHOULD BE CHECKED BEFORE USE!

example)
in terminal
$ python3 slam_start.py --kitty_sequence 6
'''

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

    '''THIS PART SHOULD BE MODIFIED, lines with # should be corrected'''

    path_to_orb_slam = "/home/jong/github_utils/ORB_SLAM2"  # TO ORB_SLAM_2
    cc_file = path_to_orb_slam + "/Examples/Monocular/mono_kitti"
    voca_file = path_to_orb_slam + "/Vocabulary/ORBvoc.txt"
    yaml_file = path_to_orb_slam + "/Examples/Monocular/" + yaml_flie_name

    img_file = "~/Downloads/data_odometry_gray/dataset/sequences/" + \
        seq_txt  # TO KITTY SEQUENCE FOLDER
    kitti_start_code = ' '.join([cc_file, voca_file, yaml_file, img_file])
    os.system(kitti_start_code)


def start():
    kitti_start_func()


if __name__ == "__main__":
    args = parse_args()
    seq = int(args.kitti_sequence)
    start()
