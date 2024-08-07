# ICPP-Qt - Writing GUI script with C++
An [icpp](https://github.com/vpand/icpp) module of the cross-platform GUI toolkit Qt.
 * Writing GUI script with C++ becomes so easy;
 * Supporting macOS/Linux/Windows;
 * Packed with Qt6.9.0 (6.0.4 for Intel macOS/Linux);

```sh
C:\vpand>icpp qt
Module Qt6(v1.0.0) usage: icpp qt -- script.cc [argv...]
```

**Follow the one-by-one step to build your own version of icpp-qt module.**

## Clone
Clone the qt main repository:
```sh
mkdir build
git clone https://code.qt.io/qt/qt5.git build/qt6
cd build/qt6
```

Clone the submodule repositories:
### Unix
ARM Chip on macOS/Linux:
```sh
git switch 6.9
perl init-repository --module-subset=qtbase,qtshadertools,qtdeclarative,qthttpserver
```
Intel Chip on macOS/Linux:
```sh
git switch 6.0
perl init-repository --module-subset=qtbase,qtshadertools,qtsvg,qtdeclarative
```

### Windows
```sh
git switch 6.9
.\init-repository.bat --module-subset=qtbase,qtshadertools,qtdeclarative,qthttpserver
```

## Build
Set the current working directory as ICPP-QT/build:
```sh
cd ..
```

### macOS
Building on macOS:
```sh
./qt6/configure -no-framework -prefix $PWD/install
cmake --build . --parallel 8
cmake --install .
```

### Linux
Building on linux:
```sh
sudo apt-get install libx11-*
sudo apt-get install libx11*
sudo apt-get install libxcb-*
sudo apt-get install libxcb*
sudo apt-get install libxkbcommon-dev
sudo apt-get install libxkbcommon-x11-dev
sudo apt-get install libgl-dev libegl-dev libfontconfig1-dev libinput-dev
./qt6/configure -xcb -prefix $PWD/install
cmake --build . --parallel 8
cmake --install .
```
Linking the fonts:
```sh
# build/install/lib
ln -s ../../../fonts .
```

### Windows
Make sure to open the correct command prompt (e.g. 'x64 Native Tools Command Prompt for VS 2022') which properly sets up the needed environment variables. Also make sure that Ninja can be found (by adding the path to ninja,exe to your PATH env var)

Building on windows:
```sh
.\qt6\configure.bat -prefix %cd%/install
cmake --build .
cmake --install .
```

## Post Build
If you switched to another qt version, you should copy the exist icpp module configuration qt-*.json to a new file, and modify the binary-libs list which can be generated as follows:
```sh
icpp post-build.cc
```

## Pack
Pack an icpp module(e.g.: qt-osx.json):
```sh
imod --create=./icpp-x.json
```

## Install
Install the icpp-qt module(e.g.: qt-osx-arm64.icpp):
```sh
imod --install=./qt-x-x.icpp
```

## Test
Don't miss the "--", it's important to make the qt related script running correctly. The following command arguments mean: run the qt module with the script file argument. The script will be running under the control of the qt module.

Run a qt related icpp script:
```sh
icpp qt -- test/helloworld.cc
```
![helloworld-win](https://raw.githubusercontent.com/vpand/icpp-qt/main/image/helloworld-win.png)
![helloworld-mac](https://raw.githubusercontent.com/vpand/icpp-qt/main/image/helloworld-mac.png)
![helloworld-linux](https://raw.githubusercontent.com/vpand/icpp-qt/main/image/helloworld-linux.png)

## Reference
 * [Building Qt6 from Git](https://wiki.qt.io/Building_Qt_6_from_Git)

## Issue
If you encounter any problems when using icpp, before opening an issue, please check the [Bug Report](https://github.com/vpand/icpp/blob/main/.github/ISSUE_TEMPLATE/bug_report.md) template, and provide as many details as you can. Only if we can reproduce the problem, we can then solve it.
