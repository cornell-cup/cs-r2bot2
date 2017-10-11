# RPLIDAR
# https://slamtec.com

# RPLIDAR is needed for the LIDAR to work
# 1. Head to https://www.slamtec.com/download/lidar/sdk/rplidar_sdk_v1.5.7.zip
# 2. Download and extract to 'lidar'


import subprocess
import platform

if __name__=="__main__":
    if "Windows" in platform.system():
        subprocess.call([ "wget", "https://www.slamtec.com/download/lidar/sdk/rplidar_sdk_v1.5.7.zip" ])
        subprocess.call([ "unzip", "rplidar_sdk_v1.5.7.zip", "-d", "rplidar" ])
        subprocess.call([ "rm", "rplidar_sdk_v1.5.7.zip" ])
    elif "Linux" in platform.system():
        subprocess.call([ "wget", "https://www.slamtec.com/download/lidar/sdk/rplidar_sdk_v1.5.7.zip" ])
        subprocess.call([ "unzip", "rplidar_sdk_v1.5.7.zip", "-d", "rplidar" ])
        subprocess.call([ "rm", "rplidar_sdk_v1.5.7.zip" ])

