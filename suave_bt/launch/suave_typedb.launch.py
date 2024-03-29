# Copyright 2023 Gustavo Rezende Silva
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():
    pkg_metacontrol_kb = get_package_share_directory(
        'metacontrol_kb')

    pkg_metacontrol_bringup = get_package_share_directory(
        'metacontrol_bringup')
    metacontrol_bringup_launch_path = os.path.join(
        pkg_metacontrol_bringup,
        'launch',
        'metacontrol_bringup.launch.py')

    pkg_suave_path = get_package_share_directory(
        'suave')
    suave_launch_path = os.path.join(
        pkg_suave_path,
        'launch',
        'suave.launch.py')

    schema_path = "[{0}, {1}]".format(
        os.path.join(pkg_metacontrol_kb, 'config', 'schema.tql'),
        os.path.join(pkg_metacontrol_kb, 'config', 'ros_schema.tql'))

    data_path = "[{}]".format(
        os.path.join(pkg_metacontrol_kb, 'config', 'suave.tql'))

    metacontrol_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(metacontrol_bringup_launch_path),
        launch_arguments={
            'schema_path': schema_path,
            'data_path': data_path,
            'database_name': 'suave_db',
            'force_data': 'True',
            'force_database': 'True',
        }.items()
    )

    suave_bt_node = Node(
        package='suave_bt',
        executable='mock_suave',
    )

    suave_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(suave_launch_path),
        launch_arguments={
            'task_bridge': 'False'}.items()
    )

    return LaunchDescription([
        metacontrol_bringup,
        suave_bt_node,
        suave_launch
    ])
