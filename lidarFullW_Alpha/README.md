## Synopsis

Contains code/project notes/ and Data for GEO+CS lidar data processing

## Installing Google Test
### 1. Download the repo

Using Git, clone the repo from the Google Test GitHub page:
```
$mkdir -p ~/Documents/repos
$cd ~/Documents/repos
$git clone https://github.com/google/googletest.git
```

### 2. Use the commands in the README file

Check the README file in:

`~/Documents/repos/googletest/googletest/README.md`

First set the variable GTEST_DIR:

```
$GTEST_DIR=~/Documents/repos/googletest/googletest
$echo ${GTEST_DIR}
```

Move to the directory:

`$cd ~/Documents/repos/googletest/googletest/`

Now run the following commands from ~/build/googletest/googletest/README.md:
```
$g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc 
$ar -rv libgtest.a gtest-all.o
```

### 3. Check the location of the libgtest file

There should be a file libgtest.a in the directory ~/Documents/repos/googletest/googletest/:
```
$cd ~/Documents/repos/googletest/googletest/
$ls *.a
```

### 4. Check the installation worked

Helpfully, a sample test has been created:

```
$cd ${GTEST_DIR}/make
$make
$./sample1_unittest
```
