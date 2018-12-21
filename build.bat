SET build_dir=build
SET install_dir=%build_dir%/bin/
SET config=Debug

cd %build_dir%
cmake ../cmake/ -DCMAKE_INSTALL_PREFIX=%install_dir% -DCMAKE_BUILD_TYPE=%config%
cmake --build . --target install
cd ..
