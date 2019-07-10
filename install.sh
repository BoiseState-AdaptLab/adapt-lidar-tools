#!/bin/bash
# Author: Ravi 
# Installer script to setup adapt-lidar-tools and build the executables
set -e
set -u

### Prints help message
help_message(){
  printf "\nUsage: \n\t$(basename $0) <[-d] [-c dir] [-h]>\n"
  printf "where:\n"
  printf "\t-d\tInstalls executable in the default bin/ directory\n"
  printf "\t-c dir\tInstall executable in user specified directory dir (full path required)\n"
  printf "\t-h\tPrints usage\n\n"
  exit 0;
}

### Initializes and updates submodules
init_update_modules(){
  ## Initialize and update submodules
  printf "Initializing submodules\n"
  git submodule init
  printf "####                      (25%%)\n"
  sleep 2

  echo "Updating submodules"
  git submodule update
  printf "###########               (50%%)\n"
  sleep 2

  ## Change 'atoill' to 'atoll'
  echo "Updating PulseWaves"
  cd deps/PulseWaves/src
  sed -i 's/atoill/atoll/g' pulsefilter.cpp
  sed -i 's/atoill/atoll/g' pulsetransform.cpp
  make libpulsewaves.a
  printf "##############            (75%%)\n"
  sleep 2
}

### Builds all executables
build_execs(){
  ## Build the pls-to-geotiff driver
  echo "Building geotiff-driver in bin/"
  cd ../../../
  make geotiff-driver
  printf "################          (90%%)\n"
  sleep 2

  ## Build the pls-info tool
  echo "Building pls-info tool in bin/"
  make pls-info
  printf "###################       (100%%)\n\n"
  sleep 2
}

# Copy executables to user specified directory
copy_exec_to_dir(){
  if [[ ! -d "$dir" ]]
  then
    echo "Directory doesn't exist. Creating now"
    mkdir -p -- "$dir"
    echo "Directory created"
  fi 
  cp bin/* "$dir" 
 
}

### 'trap' defines and activates handlers to be run when the shell receives 
### signals or other special conditions
trap 'this_command=$BASH_COMMAND; previous_command=$this_command' DEBUG

### Abort on a failed command
trap 'printf "\nexit $? due to:\n\t$previous_command\n"' EXIT

# Exit if no options
if [ $# -eq 0 ];
then
  help_message
  exit 0
fi

# Parse options
while getopts 'dc:h' OPTION; do
  case "$OPTION" in
    d)
      echo "Using default options."
      init_update_modules
      build_execs
      printf "Success!\n\n"
      printf "The executables can be found in $(pwd)/bin\n"
      printf "To add the bin/ directory to your environment path add the following line to your bash profile: \n"
      printf "\texport PATH=\$PATH:$(pwd)/bin\n\n"
      printf "Installation complete!\n\n"
      exit 0
      ;;

    h)
      help_message
      exit 0;
      ;;

    c)
      dir="$OPTARG"
      echo "The directory provided is $OPTARG"
      init_update_modules
      build_execs
      copy_exec_to_dir
      printf "Success!\n\n"
      printf "Installation complete!\n\n"
      exit 0
      ;;
    ?)
	help_message
	exit 1
      ;;
  esac
done

# Check for invalid options
if [ $OPTIND -eq 1 ]; 
then 
  echo "Invalid option/ No option specified"; 
  help_message;
fi

# Removes all the options that have been parsed by getopts from the 
# parameters list, and so after that point, $1 will refer to the first 
# non-option argument passed to the script.
shift "$(($OPTIND -1))"
exit 1

