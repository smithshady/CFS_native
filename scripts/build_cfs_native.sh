# Build native cfs and cd to the executable directory
cd ..
make SIMULATION=native prep
make install
cd build/exe/cpu1/
