cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -B ./cmake-build-release -S ./
cd ./cmake-build-release && ninja -j 32
./Assignment_1_RUNNER
cd ..
