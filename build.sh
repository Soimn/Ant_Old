build_dir=build
install_dir=./bin
config=Debug

mkdir -p $build_dir
pushd $build_dir > /dev/null
cmake ../cmake/ -DCMAKE_INSTALL_PREFIX=$install_dir -DCMAKE_BUILD_TYPE=$config
cmake --build . --target install
popd > /dev/null
