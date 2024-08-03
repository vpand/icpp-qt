/* An icpp module of the cross-platform GUI toolkit Qt */
/* By Jesse Liu < neoliu2011@gmail.com >, 2024 */
/* Copyright (c) vpand.com 2024. This file is released under LGPL2.
   See LICENSE in root directory for more details
*/

/*
This is an icpp script to clone the qt modules, usage:
    icpp clone.cc [branch]

The default branch is 6.8 .
*/

#include <icpp.hpp>
#include <icppex.hpp>

#define log(fmt, ...)                                                          \
  {                                                                            \
    icpp::prints(fmt, __VA_ARGS__);                                            \
    std::cout.flush();                                                         \
  }

constexpr std::string_view branches[] = {
    "5.15",  "6.2",   "6.6",   "6.6.3", "6.7",
    "6.7.0", "6.7.1", "6.7.2", "6.8",   "dev",
};

constexpr std::string_view branch_default = "6.8";

constexpr std::string_view modules_default[] = {
    "qtbase",
    "qtcharts",
    "qtftp",
    "qtgraphicaleffects",
    "qtgraphs",
    "qtgrpc",
    "qthttp",
    "qthttpserver",
    "qtimageformats",
    "qtmqtt",
    "qtmultimedia",
    "qtnetworkauth",
    "qtqa",
    "qtquick1",
    "qtscript",
    "qtscxml",
    "qtserialbus",
    "qtserialport",
    "qtshadertools",
    "qtspeech",
    "qtstatemachines",
    "qtstyleplugins",
    "qtsvg",
    "qtsystems",
    "qttranslations",
    "qtvirtualkeyboard",
    "qtvoiceassistant",
    "qtwayland",
    "qtwebchannel",
    "qtwebengine",
    "qtwebglplugin",
    "qtwebsockets",
    "qtwebview",
    "qtx509",
    "qtxmlpatterns",
#if __LINUX__
    "qtx11extras",
#elif __WIN__
    "qtwinextras",
#endif
};

auto git_clone(std::string_view module, std::string_view branch,
               std::string_view repodir) {
  auto repo = std::format("{}/{}", repodir, module);
  if (fs::exists(repo)) {
    icpp::prints("Ignored cloning {}, the repository {} exists.\n", module,
                 repo);
    return;
  }
  log("Cloning {}...\n", module);
  icppex::command("git",
                  {"clone", "--recurse-submodules", "--depth=1",
                   std::format("https://code.qt.io/qt/{}.git", module), repo});
  log("Checking {} out...\n", module);
  icppex::command("git", {"checkout", branch.data(), "--", repo.data()});
}

int main(int argc, const char *argv[]) {
  auto branch = branch_default;
  if (argc > 1) {
    for (auto &b : branches) {
      if (b == argv[1]) {
        icpp::prints("Using branch {}.\n", b);
        branch = argv[1];
        break;
      }
    }
  }

  auto thisdir = fs::absolute(argv[0]).parent_path();
  auto repodir = thisdir / "module";
  if (!fs::exists(repodir))
    fs::create_directory(repodir);

  for (auto &m : modules_default)
    git_clone(m, branch, repodir.string());
  std::puts("Done.");
  return 0;
}
