/* An icpp module of the cross-platform GUI toolkit Qt */
/* By Jesse Liu < neoliu2011@gmail.com >, 2024 */
/* Copyright (c) vpand.com 2024. This file is released under LGPL2.
   See LICENSE in root directory for more details
*/

/*
This is an icpp module main entry to run the qt related script, usage:
1.install this module:
  imod --install=qt-*-*.icpp
2.run qt related script with the qt module:
  icpp qt -- helloworld.cc
*/

#include <QtWidgets/QApplication>
#include <icpp.hpp>
#include <icppex.hpp>

constexpr bool logging = true;

#define log(format, ...)                                                       \
  if (logging) {                                                               \
    icpp::prints(format, __VA_ARGS__);                                         \
  }

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    icpp::prints("Qt Module usage: icpp qt -- /path/to/source.cc [argv...]\n");
    return -1;
  }
  std::string_view srcfile{argv[1]};

  auto qtbin = std::string(icpp::home_directory()) + "/.icpp/bin/qt";
  auto moc = qtbin + "/moc";
#if __WIN__
  moc += ".exe";
#endif
  auto srcname = fs::path(srcfile).stem().string();
  auto hdrfile =
      (fs::path(srcfile).parent_path() / std::format("{}.h", srcname)).string();
  auto mocfile =
      (fs::path(srcfile).parent_path() / std::format("{}.moc.cc", srcname))
          .string();
  if (fs::exists(hdrfile)) {
    log("Generating moc source: {}\n", mocfile);
    fs::remove(mocfile);
    auto mocouts = icppex::execute(moc, {"-o", mocfile.data(), hdrfile.data()});
    if (!fs::exists(mocfile)) {
      std::puts(mocouts.data());
      return -2;
    }
  }

  auto qtlib = std::string(icpp::home_directory()) + "/.icpp/lib/qt";
  log("Set Qt library path: {}\n", qtlib);
  QApplication::addLibraryPath(qtlib.data());

  if (fs::exists(mocfile)) {
    log("Executing moc source: {}\n", mocfile);
    icpp::exec_source(mocfile);
  }

  log("Executing user source: {}\n", srcfile);
  return icpp::exec_source(srcfile, argc - 1, &argv[1]);
}
