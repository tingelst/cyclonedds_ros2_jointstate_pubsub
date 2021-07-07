# cyclonedds_ros2_jointstate_pubsub
Simple joint state publish and subscriber using CycloneDDS and ROS2 Galactic

## Clone and build
```bash
git clone https://github.com/tingelst/cyclonedds_ros2_jointstate_pubsub.git
cd cyclonedds_ros2_jointstate_pubsub
mkdir build && cd build 
source /opt/ros/galactic/setup.bash
cmake ..
make
```

## Run

Terminal 1:
```bash
source /opt/ros/galactic/setup.bash
ros2 topic echo /joint_states
```

Terminal 2:
```bash
cd build
./publisher
```

Terminal 3:
```bash
cd build
./subscriber
```
