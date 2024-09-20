import os
import yaml

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    common_params_file = os.path.join(
        get_package_share_directory("local_planner"),
        "config",
        "common_params.yaml",
        # get_package_share_directory("local_planner"), "config", "common_params_v3.yaml"
    )
    with open(common_params_file, "r") as file:
        params = yaml.safe_load(file)

    twoWayDrive = LaunchConfiguration("twoWayDrive")
    maxSpeed = LaunchConfiguration("maxSpeed")
    autonomyMode = LaunchConfiguration("autonomyMode")
    autonomySpeed = LaunchConfiguration("autonomySpeed")
    joyToSpeedDelay = LaunchConfiguration("joyToSpeedDelay")
    goalX = LaunchConfiguration("goalX")
    goalY = LaunchConfiguration("goalY")

    declare_twoWayDrive = DeclareLaunchArgument(
        "twoWayDrive", default_value=str(params["twoWayDrive"]), description=""
    )
    declare_maxSpeed = DeclareLaunchArgument(
        "maxSpeed", default_value=str(params["maxSpeed"]), description=""
    )
    declare_autonomyMode = DeclareLaunchArgument(
        "autonomyMode", default_value=str(params["autonomyMode"]), description=""
    )
    delcare_autonomySpeed = DeclareLaunchArgument(
        "autonomySpeed", default_value=str(params["autonomySpeed"]), description=""
    )
    declare_joyToSpeedDelay = DeclareLaunchArgument(
        "joyToSpeedDelay", default_value=str(params["joyToSpeedDelay"]), description=""
    )
    declare_goalX = DeclareLaunchArgument("goalX", default_value="0.0", description="")
    declare_goalY = DeclareLaunchArgument("goalY", default_value="0.0", description="")

    local_planner_node = Node(
        package="local_planner",
        executable="localPlanner",
        name="localPlanner",
        output="screen",
        parameters=[
            {
                "pathFolder": os.path.join(
                    get_package_share_directory("local_planner"),
                    "paths",
                    "original",
                    # get_package_share_directory("local_planner"), "paths", "v3"
                ),
                "twoWayDrive": twoWayDrive,
                "maxSpeed": maxSpeed,
                "autonomyMode": autonomyMode,
                "autonomySpeed": autonomySpeed,
                "joyToSpeedDelay": joyToSpeedDelay,
                "goalX": goalX,
                "goalY": goalY,
            },
            os.path.join(
                get_package_share_directory("local_planner"),
                "config",
                "local_planner.yaml",
                # "local_planner_v3.yaml",
            ),
        ],
    )

    path_follower_node = Node(
        package="local_planner",
        executable="pathFollower",
        name="pathFollower",
        output="screen",
        parameters=[
            {
                "twoWayDrive": twoWayDrive,
                "maxSpeed": maxSpeed,
                "autonomyMode": autonomyMode,
                "autonomySpeed": autonomySpeed,
                "joyToSpeedDelay": joyToSpeedDelay,
            },
            os.path.join(
                get_package_share_directory("local_planner"),
                "config",
                "path_follower.yaml",
                # "path_follower_v3.yaml",
            ),
        ],
    )

    ld = LaunchDescription()

    # Add the actions
    ld.add_action(declare_twoWayDrive)
    ld.add_action(declare_maxSpeed)
    ld.add_action(declare_autonomyMode)
    ld.add_action(delcare_autonomySpeed)
    ld.add_action(declare_joyToSpeedDelay)
    ld.add_action(declare_goalX)
    ld.add_action(declare_goalY)

    ld.add_action(local_planner_node)
    ld.add_action(path_follower_node)
    return ld
