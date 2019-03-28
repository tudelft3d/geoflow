#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <optional>
#include <cstdlib>
#include <cstring>
#include "osdialog.hpp"

std::optional<std::string> osdialog_file(osdialog_file_action action, const char* filename, const char* filters) {
  osdialog_filters* filters_c=nullptr;
  if (filters)
    filters_c = osdialog_filters_parse(filters);
  
  std::optional<std::string> result;
  char *filepath_c = osdialog_file(action, nullptr, filename, filters_c);
  if (filepath_c) {
    result = std::string(filepath_c);
    std::free(filepath_c);
    osdialog_filters_free(filters_c);
  }
  return result;
};

namespace ImGui {

  void FilePicker(osdialog_file_action action, std::string& picked_file, const char* filters) {
    if (ImGui::Button("Open")) {
      auto result = osdialog_file(action, nullptr, filters);
      if (result.has_value())
        picked_file = result.value();
    }
    ImGui::SameLine();
    ImGui::Text("%s", picked_file.c_str());
  }
}