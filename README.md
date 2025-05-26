# Chess robot

## Before running any program source the setup.bash


```
source install/setup.bash 
```

## To use just the vision main run

```
ros2 run ur5_sem2_scripts vision_main --ros-args -p camera_index:=<insert index>
```

## To use the final_main run
```
ros2 run ur5_sem2_scripts final_main --ros-args -p camera_index:=<insert index>
```
## To use with robot run

for starting communication with robot:
```
ros2 launch ur_robot_driver ur_control.launch.py ur_type:=ur5 robot_ip:=192.168.1.54

```

for starting moveit moveplanner:
```
ros2 launch ur_moveit_config ur_moveit.launch.py ur_type:=ur5 launch_rviz:=true
```

for starting svejse_frame run:

```
ros2 run ur5_sem2_scripts svejse_frame 
```

for starting robot_main run:

```
ros2 run ur5_sem2_scripts robot_main
```

## Additional parameters

start debug mode:
```
--log-level DEBUG 
```

# For Pico R/W access
sudo chmod a+rw /dev/ttyACM0


