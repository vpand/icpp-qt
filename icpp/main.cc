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

constexpr bool logging = false;

#define log(format, ...)                                                       \
  if (logging) {                                                               \
    icpp::prints(format, __VA_ARGS__);                                         \
  }

#if __WIN__
#define EXE_EXT ".exe"
#else
#define EXE_EXT ""
#endif

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    icpp::prints("Module Qt6(v1.0.0) usage: icpp qt -- script.cc [argv...]\n");
    return -1;
  }

  // source
  std::string_view srcfile{argv[1]};
  auto srcname = fs::path(srcfile).stem().string();
  // header
  auto hdrfile =
      (fs::path(srcfile).parent_path() / std::format("{}.h", srcname)).string();
  // source.moc
  auto mocfile =
      (fs::path(srcfile).parent_path() / std::format("{}.moc.cc", srcname))
          .string();
  auto hasmoc = false, updated = true;
  if (fs::exists(hdrfile)) {
    if ((hasmoc = fs::exists(mocfile))) {
      // update check
      auto moctm = fs::last_write_time(mocfile);
      if (moctm > fs::last_write_time(srcfile) &&
          moctm > fs::last_write_time(hdrfile)) {
        log("{}\n", "Using the old moc source, no need to update.");
        updated = false;
      }
    }
    if (updated) {
      log("Generating moc source: {}\n", mocfile);
      auto moc =
          std::string(icpp::home_directory()) + "/.icpp/bin/qt/moc" EXE_EXT;
      fs::remove(mocfile);
      auto mocouts =
          icppex::execute(moc, {"-o", mocfile.data(), hdrfile.data()});
      if (!(hasmoc = fs::exists(mocfile))) {
        std::puts(mocouts.data());
        return -2;
      }
      // include the original source into this newly created moc source
      std::fstream mocfs(mocfile, std::ios::in | std::ios::out | std::ios::ate);
      auto incsrc = std::format("\n#include \"{}\"\n",
                                fs::path(srcfile).filename().string());
      mocfs.write(incsrc.data(), incsrc.size());
    }
  }

  int targc = argc - 1;
  auto targv = &argv[1];
  auto platform =
      std::string(icpp::home_directory()) + "/.icpp/lib/qt/plugins/platforms";
#if __UNIX__
  setenv("QT_QPA_PLATFORM_PLUGIN_PATH", platform.data(), true);
#else
  SetEnvironmentVariable("QT_QPA_PLATFORM_PLUGIN_PATH", platform.data());
#endif
  if (hasmoc) {
    log("Executing user.moc source: {}\n", mocfile);
    return icpp::exec_source(mocfile, targc, targv);
  }

  log("Executing user source: {}\n", srcfile);
  return icpp::exec_source(srcfile, targc, targv);
}
