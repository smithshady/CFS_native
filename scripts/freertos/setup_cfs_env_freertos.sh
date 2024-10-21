export SIMULATION=xc32-samx7x
export OSAL_SYSTEM_BSPTYPE=xc32-samx7x
export OSAL_SYSTEM_OSTYPE=freertos
export CFS_BASE_DIR=$HOME/dev/cFS-submoduled
export XC32_TOOLCHAIN_PATH=/opt/microchip/xc32/v4.40
export MISSION_HOME=$CFS_BASE_DIR/roverlink
export MISSION_NAME=roverlink
export OSAL_EXT_SOURCE_DIR=$MISSION_HOME/osal/src/os/external-dependencies

#cd ../
#git submodule update --init --recursive
#cd apps/sbn
#./install ../..
#cd ../..
#cd ${MISSION_HOME}/apps/ci/fsw/examples/
#./setup.sh udp
#cd ${MISSION_HOME}/apps/to/fsw/examples/
#./setup.sh udp 
#cd ${MISSION_HOME}

