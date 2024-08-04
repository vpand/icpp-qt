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

#if __UNIX__
#if __APPLE__
void rpath_reset(std::string_view file) {
  std::vector<std::string> args;
  args.push_back("-rpath");
  args.push_back(file.find(".framework") != std::string_view::npos
                     ? "@loader_path/../../../"
                     : "@loader_path/../../lib");
  args.push_back("@loader_path/");
  for (auto &line : icppex::command2("otool", {"-l", file.data()})) {
    // name @rpath/QtWidgets.framework/Versions/A/QtWidgets
    auto parts = icpp::split(line, " @rpath/");
    if (parts.size() == 2) {
      if (line.find('.') == std::string::npos)
        continue;
      auto name = icpp::split(parts[1], ".")[0];
      args.push_back("-change");
      args.push_back(std::format("@rpath/{0}.framework/Versions/A/{0}", name));
      args.push_back(std::format("@rpath/{}", name));
    }
  }
  args.push_back(file.data());
  icpp::prints("Reset rpath {}.\n", file);
  icppex::command("install_name_tool", args);
}
#endif

void strip(std::string_view file) {
#if __APPLE__
  rpath_reset(file);
#endif
  icpp::prints("Stripped {}.\n", file);
  icppex::command("strip", {"-x", file.data()});
}
#endif

int main(int argc, const char *argv[]) {
  auto thisdir = fs::absolute(argv[0]).parent_path();
  auto installdir = thisdir / "build/install";

#if __UNIX__
  // strip moc
  strip((installdir / "libexec/moc").string());
#endif
#if __APPLE__
  // strip/list framework
  std::vector<std::string> frameworks, plugins;
  for (auto &entry : fs::directory_iterator(installdir / "lib")) {
    if (entry.is_directory()) {
      auto dir = entry.path();
      if (dir.string().ends_with(".framework")) {
        frameworks.push_back(dir.stem().string());
        strip((dir / dir.stem()).string());
      }
    }
  }
  for (auto &entry : fs::recursive_directory_iterator(installdir / "plugins")) {
    auto path = entry.path().string();
    if (path.ends_with(".dylib")) {
      plugins.push_back(icpp::split(path, "plugins/")[1]);
      strip(path);
    }
  }
  auto hdrs = ""s, libs = ""s, plugs = ""s;
  for (auto &f : frameworks) {
    hdrs += std::format("    \"build/install/lib/{}.framework/Headers\",\n", f);
    libs += std::format("    \"build/install/lib/{}.framework/{}\",\n", f, f);
  }
  for (auto &p : plugins)
    plugs += std::format("    \"build/install/plugins/{}\",\n", p);
  icpp::prints("header-dirs:\n{}\nbinary-libs:\n{}\n{}\n", hdrs, libs, plugs);
#endif
  return 0;
}
