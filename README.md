# icpp-qt
An icpp module of the cross-platform GUI toolkit Qt.

## Clone
Clone the qt main repository:
```sh
mkdir build
git clone --depth=1 https://code.qt.io/qt/qt5.git build/qt6
cd build/qt6
```

Clone the submodule repositories:
### Unix
```sh
perl init-repository --module-subset=qtbase,qtshadertools,qtdeclarative,qthttpserver
```

### Windows
```sh
.\init-repository.bat --module-subset=qtbase,qtshadertools,qtdeclarative,qthttpserver
```

## Build
Set the current working directory as ICPP-QT/build:
```sh
cd ..
```

### Unix
Building on unix-like system:
```sh
./qt6/configure -prefix $PWD/install
cmake --build . --parallel 8
cmake --install .
```

### Windows
Make sure to open the correct command prompt (e.g. 'x64 Native Tools Command Prompt for VS 2022') which properly sets up the needed environment variables. Also make sure that Ninja can be found (by adding the path to ninja,exe to your PATH env var)

Building on windows:
```sh
.\qt6\configure.bat -prefix %cd%/install
cmake --build .
cmake --install .
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

## Reference
 * [Building Qt6 from Git](https://wiki.qt.io/Building_Qt_6_from_Git)
