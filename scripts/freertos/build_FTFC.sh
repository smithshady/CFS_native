#!/bin/bash

# Start Clock
starttime=`date +%s`

# Display Help
Help() {
  echo ""
  echo "Usage Notes:"
  echo "  - {git fetch --tags} should be called before running the script"
  echo "  - Add an argument after -m to specify mission number (1 or 2)"
  echo "  - OS is detected automatically but can use -l or -w to force linux or windows (git bash)"
  echo "  - Use the -f option for fake fpga and disabled memory"
}

# Defaults
Mission=1
OpSys="Windows"
fakeFPGA=false

# Detect operating system
if [[ $(grep MINGW /proc/version) ]]; then
  echo "Git Bash has been detected."
  OpSys="Windows"
else
  echo "Linux has been detected"
  OpSys="Linux"
fi

# Check Options and Arguments
while getopts :hflwm: option; do
  case $option in
    h) Help
       exit;;
    f) fakeFPGA=true;;
    l) OpSys="Linux";;
    w) OpSys="Windows";;
    m) Mission=$OPTARG;;
   \?) echo "Error: Invalid option"
       exit 1;;
  esac
done

# Print Mission
#echo ""
#if [ $Mission != 1 ] && [ $Mission != 2 ]; then
#  echo "Error: Invalid Mission Number (please select either 1 or 2)"
#  exit 1
#else
#  printf "Mission: $Mission"
#fi

# Get Version Number
version=$(git describe --tags --abbrev=0)
if [[ $version =~ "-main" ]]; then
  version=${version::-5}
fi
version+="-local"
echo " | Version: $version"
echo ""

# Move to Correct Directory
dir=${PWD##*/}
if [ "$dir" = "scripts" ]; then
  dir="roverlink"
  cd ..
fi
if [ "$dir" = "roverlink" ]; then
  cd cFS.X
else
  echo "Error: Please run script from firmware or script directory"
  exit 1
fi

# Clean project
printf "Cleaning Project Files... "
rm -rf build dist .generated_files debug
echo "Done."

# Generate New Makefiles
printf "Generating Make Files... "
if [ "$OpSys" = "Windows" ]; then
  "C:/Program Files/Microchip/MPLABX/v5.50/mplab_platform/bin/prjMakefilesGenerator.bat" .
else
  "/opt/microchip/mplabx/v5.50/mplab_platform/bin/prjMakefilesGenerator.sh" .
fi
echo "Done."
echo ""

# Build
echo "STARTING BUILD..."
if [ "$OpSys" = "Windows" ]; then
  if [ $fakeFPGA == false ]; then
      "C:/Program Files/Microchip/MPLABX/v5.50/gnuBins/GnuWin32/bin/make.exe" \
      -j8 CONF=FTFC MP_EXTRA_CC_PRE="-DVERSION_NUMBER=$version -DMISSION_NUMBER=0x1111"
  else
      "C:/Program Files/Microchip/MPLABX/v5.50/gnuBins/GnuWin32/bin/make.exe" \
      -j8 CONF=sam_v71_xult MP_EXTRA_CC_PRE="-DVERSION_NUMBER=$version -DMISSION_NUMBER=0x1111 -DDISABLE_DEPLOYED_MEMORY_FLAG=1 -DTHERMAL_DRIVER_USE_FAKE_FPGA=1 -DMD_USE_FAKE_ENCODERS=1"
  fi
else
  if [ $fakeFPGA == false ]; then
      "/opt/microchip/mplabx/v5.50/mplab_platform/bin/make" \
      -j8 CONF=FTFC MP_EXTRA_CC_PRE="-DVERSION_NUMBER=$version -DMISSION_NUMBER=0x1111"
  else
      "/opt/microchip/mplabx/v5.50/mplab_platform/bin/make" \
      -j8 CONF=FTFC MP_EXTRA_CC_PRE="-DVERSION_NUMBER=$version -DMISSION_NUMBER=0x1111 -DDISABLE_DEPLOYED_MEMORY_FLAG=1 -DTHERMAL_DRIVER_USE_FAKE_FPGA=1 -DMD_USE_FAKE_ENCODERS=1"
  fi
fi

# Check Success
if [ $? == 0 ]
then
  echo ""
  echo "BUILD SUCCESS"
fi

# Print Build Time
endtime=`date +%s`
runtime=$((endtime-starttime))
cd ..
echo "BUILD TIME: $runtime SECONDS"

