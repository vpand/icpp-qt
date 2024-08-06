/* An icpp module of the cross-platform GUI toolkit Qt */
/* By Jesse Liu < neoliu2011@gmail.com >, 2024 */
/* Copyright (c) vpand.com 2024. This file is released under LGPL2.
   See LICENSE in root directory for more details
*/

/*
This script helps to write the icpp-qt module packing json configuration.
i.e: strip binaries, generate include/library list, etc.
*/

#include <icpp.hpp>
#include <icppex.hpp>

#define QTVERSION "6"

#if __APPLE__
#define SHLIB_EXT ".dylib"
#elif __WIN__
#define SHLIB_EXT ".dll"
#else
#define SHLIB_EXT ".so"
#endif

#if __UNIX__
#if __APPLE__
#define SHLIB_VEREXT QTVERSION SHLIB_EXT
#else
#define SHLIB_VEREXT SHLIB_EXT "." QTVERSION
#endif

void strip(std::string_view thisdir, std::string_view file) {
  icpp::prints("Stripped {}.\n", file);
  icppex::command("strip", {"-x", file.data()});
#if __APPLE__
  icpp::prints("Codesigned {}.\n", file);
  icppex::command("codesign", {"--force", "--sign", "-", file.data()});
#endif
}
#endif

int main(int argc, const char *argv[]) {
  auto thisdir = fs::absolute(argv[0]).parent_path();
  auto installdir = thisdir / "build/install";

#if __UNIX__
  // strip moc
  strip(thisdir.string(), (installdir / "libexec/moc").string());

  // strip/list libs
  std::vector<std::string> qtlibs, plugins;
  for (auto &entry : fs::directory_iterator(installdir / "lib")) {
    auto path = entry.path().string();
    if (path.ends_with(SHLIB_VEREXT)) {
      qtlibs.push_back(icpp::split(path, "lib/")[1]);
      strip(thisdir.string(), path);
    }
  }
  for (auto &entry : fs::recursive_directory_iterator(installdir / "plugins")) {
    auto path = entry.path().string();
    if (path.ends_with(SHLIB_EXT)) {
      plugins.push_back(icpp::split(path, "plugins/")[1]);
      strip(thisdir.string(), path);
    }
  }
  auto libs = ""s, plugs = ""s;
  for (auto &l : qtlibs)
    libs += std::format("    \"build/install/lib/{}\",\n", l);
  for (auto &p : plugins)
    plugs += std::format("    \"build/install/plugins/{}\",\n", p);

#else
  // list libs
  std::vector<std::string> qtlibs, plugins;
  for (auto &entry : fs::directory_iterator(installdir / "bin")) {
    auto path = entry.path().string();
    if (path.ends_with(SHLIB_EXT)) {
      qtlibs.push_back(icpp::split(path, "bin\\")[1]);
    }
  }
  for (auto &entry : fs::recursive_directory_iterator(installdir / "plugins")) {
    auto path = entry.path().string();
    if (path.ends_with(SHLIB_EXT)) {
      auto &item = plugins.emplace_back(icpp::split(path, "plugins\\")[1]);
      for (auto ptr = const_cast<char *>(item.data()); *ptr; ptr++) {
        if (*ptr == '\\')
          *ptr = '/';
      }
    }
  }
  auto libs = ""s, plugs = ""s;
  for (auto &l : qtlibs)
    libs += std::format("    \"build/install/bin/{}\",\n", l);
  for (auto &p : plugins)
    plugs += std::format("    \"build/install/plugins/{}\",\n", p);

#endif
  icpp::prints("binary-libs:\n{}\n{}\n", libs, plugs);
  return 0;
}
