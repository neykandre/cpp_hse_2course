#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

pushd "$parent_path"
cpp-merge ./main.cpp -i ./include -s ./src -o ./cpp_contest_2_1.cpp
popd
