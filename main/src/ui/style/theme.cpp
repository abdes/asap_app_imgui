/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "ui/style/theme.h"
#include "config/config.h"
#include "ui/fonts/fonts.h"
#include "ui/fonts/material_design_icons.h"

#include <imgui/imgui.h>
#include <logging/logging.h>
#include <toml++/toml.hpp>

#include <array>
#include <cstring>
#include <fstream>
#include <map>
#include <mutex> // for call_once()

namespace asap::ui {

const char *const Theme::LOGGER_NAME = "main";

namespace {

/// Merge in icons from Font Material Design Icons font.
ImFont *MergeIcons(float size) {
  ImGuiIO &io = ImGui::GetIO();
  // The ranges array is not copied by the AddFont* functions and is used lazily
  // so ensure it is available for duration of font usage
  static const ImWchar icons_ranges[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};

  ImFontConfig fontConfig;
  // Set Oversampling parameters to 1 on both axis, the texture will be 6 times
  // smaller. See https://github.com/ocornut/imgui/issues/1527
  fontConfig.OversampleH = 1;
  fontConfig.OversampleV = 1;
  fontConfig.MergeMode = true;
  fontConfig.PixelSnapH = true;
  auto font = io.Fonts->AddFontFromMemoryCompressedTTF(
      asap::ui::Fonts::MATERIAL_DESIGN_ICONS_COMPRESSED_DATA,
      asap::ui::Fonts::MATERIAL_DESIGN_ICONS_COMPRESSED_SIZE, size, &fontConfig,
      icons_ranges);
  // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

  return font;
}

} // namespace

void Theme::Init() {
  LoadStyle();

  //
  // Fonts
  //
  static std::once_flag init_flag;
  std::call_once(init_flag, []() {
    ImGui::GetIO().Fonts->AddFontDefault();
    MergeIcons(13.0F);
  });
}

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
void Theme::LoadDefaultStyle() {
  auto &style = ImGui::GetStyle();

  style.WindowPadding = ImVec2(5, 5);
  style.WindowRounding = 0.0F;
  style.FramePadding = ImVec2(5, 5);
  style.FrameRounding = 3.0F;
  style.ItemSpacing = ImVec2(12, 5);
  style.ItemInnerSpacing = ImVec2(5, 5);
  style.IndentSpacing = 25.0F;
  style.ScrollbarSize = 15.0F;
  style.ScrollbarRounding = 9.0F;
  style.GrabMinSize = 5.0F;
  style.GrabRounding = 3.0F;

  // clang-format off
  style.Colors[ImGuiCol_Text] = ImVec4(0.91F, 0.91F, 0.91F, 1.00F);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.40F, 0.40F, 0.40F, 1.00F);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10F, 0.10F, 0.10F, 1.00F);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00F, 0.00F, 0.00F, 0.00F);
  style.Colors[ImGuiCol_Border] = ImVec4(0.00F, 0.00F, 0.00F, 0.39F);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00F, 1.00F, 1.00F, 0.10F);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.06F, 0.06F, 0.06F, 1.00F);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75F, 0.42F, 0.02F, 0.40F);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.75F, 0.42F, 0.02F, 0.67F);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04F, 0.04F, 0.04F, 1.00F);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00F, 0.00F, 0.00F, 0.51F);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.18F, 0.18F, 0.18F, 1.00F);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15F, 0.15F, 0.15F, 1.00F);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02F, 0.02F, 0.02F, 0.53F);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31F, 0.31F, 0.31F, 0.80F);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49F, 0.49F, 0.49F, 0.80F);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49F, 0.49F, 0.49F, 1.00F);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.75F, 0.42F, 0.02F, 1.00F);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.75F, 0.42F, 0.02F, 0.78F);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.75F, 0.42F, 0.02F, 1.00F);
  style.Colors[ImGuiCol_Button] = ImVec4(0.75F, 0.42F, 0.02F, 0.40F);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.75F, 0.42F, 0.02F, 1.00F);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.94F, 0.47F, 0.02F, 1.00F);
  style.Colors[ImGuiCol_Header] = ImVec4(0.75F, 0.42F, 0.02F, 0.31F);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75F, 0.42F, 0.02F, 0.80F);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.75F, 0.42F, 0.02F, 1.00F);
  style.Colors[ImGuiCol_Separator] = ImVec4(0.61F, 0.61F, 0.61F, 1.00F);
  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75F, 0.42F, 0.02F, 0.78F);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.75F, 0.42F, 0.02F, 1.00F);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.22F, 0.22F, 0.22F, 1.00F);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.75F, 0.42F, 0.02F, 0.67F);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.75F, 0.42F, 0.02F, 0.95F);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61F, 0.61F, 0.61F, 1.00F);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00F, 0.57F, 0.65F, 1.00F);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.10F, 0.30F, 1.00F, 1.00F);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00F, 0.40F, 1.00F, 1.00F);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.75F, 0.42F, 0.02F, 0.35F);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00F, 0.00F, 0.00F, 0.94F);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.06F, 0.06F, 0.06F, 0.35F);
  // clang-format on
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

// -------------------------------------------------------------------------
// Settings load/save
// -------------------------------------------------------------------------

#define EMIT_TOML_STYLE_IMVEC2(FIELD)                                          \
  {                                                                            \
    #FIELD, toml::array {                                                      \
      style.FIELD.x, style.FIELD.y                                             \
    }                                                                          \
  }

#define EMIT_TOML_COLOR(COLOR_ID)                                              \
  {                                                                            \
    #COLOR_ID, toml::array {                                                   \
      colors[COLOR_ID].x, colors[COLOR_ID].y, colors[COLOR_ID].z,              \
          colors[COLOR_ID].w                                                   \
    }                                                                          \
  }

void Theme::SaveStyle() {

  auto &style = ImGui::GetStyle();
  auto &colors = style.Colors;

  // clang-format off
  auto root = toml::table{
    {"theme", toml::table {
        {"Key", "test"},
        {"style", toml::table{
          {"Alpha", style.Alpha},
          EMIT_TOML_STYLE_IMVEC2(WindowPadding),
          {"WindowRounding", style.WindowRounding},
          {"WindowBorderSize", style.WindowBorderSize},
          EMIT_TOML_STYLE_IMVEC2(WindowMinSize),
          EMIT_TOML_STYLE_IMVEC2(WindowTitleAlign),
          {"ChildRounding", style.ChildRounding},
          {"ChildBorderSize", style.ChildBorderSize},
          {"PopupRounding", style.PopupRounding},
          {"PopupBorderSize", style.PopupBorderSize},
          EMIT_TOML_STYLE_IMVEC2(FramePadding),
          {"FrameRounding", style.FrameRounding},
          {"FrameBorderSize", style.FrameBorderSize},
          EMIT_TOML_STYLE_IMVEC2(ItemSpacing),
          EMIT_TOML_STYLE_IMVEC2(ItemInnerSpacing),
          EMIT_TOML_STYLE_IMVEC2(TouchExtraPadding),
          {"IndentSpacing", style.IndentSpacing},
          {"ColumnsMinSpacing", style.ColumnsMinSpacing},
          {"ScrollbarSize", style.ScrollbarSize},
          {"ScrollbarRounding", style.ScrollbarRounding},
          {"GrabMinSize", style.GrabMinSize},
          {"GrabRounding", style.GrabRounding},
          EMIT_TOML_STYLE_IMVEC2(ButtonTextAlign),
          EMIT_TOML_STYLE_IMVEC2(DisplayWindowPadding),
          EMIT_TOML_STYLE_IMVEC2(DisplaySafeAreaPadding),
          {"MouseCursorScale", style.MouseCursorScale},
          {"AntiAliasedLines", style.AntiAliasedLines},
          {"AntiAliasedFill", style.AntiAliasedFill},
          {"CurveTessellationTol", style.CurveTessellationTol},
        }},
        {"colors", toml::table{
          EMIT_TOML_COLOR(ImGuiCol_Text),
          EMIT_TOML_COLOR(ImGuiCol_TextDisabled),
          EMIT_TOML_COLOR(ImGuiCol_WindowBg),
          EMIT_TOML_COLOR(ImGuiCol_ChildBg),
          EMIT_TOML_COLOR(ImGuiCol_PopupBg),
          EMIT_TOML_COLOR(ImGuiCol_Border),
          EMIT_TOML_COLOR(ImGuiCol_BorderShadow),
          EMIT_TOML_COLOR(ImGuiCol_FrameBg),
          EMIT_TOML_COLOR(ImGuiCol_FrameBgHovered),
          EMIT_TOML_COLOR(ImGuiCol_FrameBgActive),
          EMIT_TOML_COLOR(ImGuiCol_TitleBg),
          EMIT_TOML_COLOR(ImGuiCol_TitleBgActive),
          EMIT_TOML_COLOR(ImGuiCol_TitleBgCollapsed),
          EMIT_TOML_COLOR(ImGuiCol_MenuBarBg),
          EMIT_TOML_COLOR(ImGuiCol_ScrollbarBg),
          EMIT_TOML_COLOR(ImGuiCol_ScrollbarGrab),
          EMIT_TOML_COLOR(ImGuiCol_ScrollbarGrabHovered),
          EMIT_TOML_COLOR(ImGuiCol_ScrollbarGrabActive),
          EMIT_TOML_COLOR(ImGuiCol_CheckMark),
          EMIT_TOML_COLOR(ImGuiCol_SliderGrab),
          EMIT_TOML_COLOR(ImGuiCol_SliderGrabActive),
          EMIT_TOML_COLOR(ImGuiCol_Button),
          EMIT_TOML_COLOR(ImGuiCol_ButtonHovered),
          EMIT_TOML_COLOR(ImGuiCol_ButtonActive),
          EMIT_TOML_COLOR(ImGuiCol_Header),
          EMIT_TOML_COLOR(ImGuiCol_HeaderHovered),
          EMIT_TOML_COLOR(ImGuiCol_HeaderActive),
          EMIT_TOML_COLOR(ImGuiCol_Separator),
          EMIT_TOML_COLOR(ImGuiCol_SeparatorHovered),
          EMIT_TOML_COLOR(ImGuiCol_SeparatorActive),
          EMIT_TOML_COLOR(ImGuiCol_ResizeGrip),
          EMIT_TOML_COLOR(ImGuiCol_ResizeGripHovered),
          EMIT_TOML_COLOR(ImGuiCol_ResizeGripActive),
          EMIT_TOML_COLOR(ImGuiCol_PlotLines),
          EMIT_TOML_COLOR(ImGuiCol_PlotLinesHovered),
          EMIT_TOML_COLOR(ImGuiCol_PlotHistogram),
          EMIT_TOML_COLOR(ImGuiCol_PlotHistogramHovered),
          EMIT_TOML_COLOR(ImGuiCol_TextSelectedBg),
          EMIT_TOML_COLOR(ImGuiCol_DragDropTarget),
          EMIT_TOML_COLOR(ImGuiCol_NavHighlight),
          EMIT_TOML_COLOR(ImGuiCol_NavWindowingHighlight),
          EMIT_TOML_COLOR(ImGuiCol_NavWindowingDimBg),
          EMIT_TOML_COLOR(ImGuiCol_ModalWindowDimBg),
        }}
      }
    }
  };
  // clang-format on

  auto settings_path =
      asap::config::GetPathFor(asap::config::Location::F_THEME_SETTINGS);
  auto ofs = std::ofstream();
  ofs.open(settings_path.string());
  ofs << root << std::endl;
  ofs.close();
}

#define SET_COLOR_FROM_TOML(id)                                                \
  if (colors_settings[#id]) {                                                  \
    auto color = colors_settings[#id];                                         \
    colors[id] = {static_cast<float>(color[0].value<double>().value()),        \
        static_cast<float>(color[1].value<double>().value()),                  \
        static_cast<float>(color[2].value<double>().value()),                  \
        static_cast<float>(color[3].value<double>().value())};                 \
  }

void Theme::LoadStyle() {
  auto theme_settings =
      asap::config::GetPathFor(asap::config::Location::F_THEME_SETTINGS);
  auto has_config = false;
  if (!std::filesystem::exists(theme_settings)) {
    ASLOG(info, "file {} does not exist", theme_settings.string());
    LoadDefaultStyle();
    return;
  }

  try {
    auto config = toml::parse_file(theme_settings.string());
    ASLOG(info, "theme settings loaded from {}", theme_settings.string());

    if (config["theme"]) {
      auto theme = config["theme"];
      if (theme["style"]) {
        auto style = theme["style"];

        if (style["Alpha"]) {
          ImGui::GetStyle().Alpha =
              static_cast<float>(style["Alpha"].value<double>().value());
        }
        if (style["WindowPadding"]) {
          auto vec2 = style["WindowPadding"];
          ImGui::GetStyle().WindowPadding = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["WindowRounding"]) {
          ImGui::GetStyle().WindowRounding = static_cast<float>(
              style["WindowRounding"].value<double>().value());
        }
        if (style["WindowBorderSize"]) {
          ImGui::GetStyle().WindowBorderSize = static_cast<float>(
              style["WindowBorderSize"].value<double>().value());
        }
        if (style["WindowMinSize"]) {
          auto vec2 = style["WindowMinSize"];
          ImGui::GetStyle().WindowMinSize = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }
        if (style["WindowTitleAlign"]) {
          auto vec2 = style["WindowTitleAlign"];
          ImGui::GetStyle().WindowTitleAlign = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["ChildRounding"]) {
          ImGui::GetStyle().ChildRounding = static_cast<float>(
              style["ChildRounding"].value<double>().value());
        }
        if (style["ChildBorderSize"]) {
          ImGui::GetStyle().ChildBorderSize = static_cast<float>(
              style["ChildBorderSize"].value<double>().value());
        }
        if (style["PopupRounding"]) {
          ImGui::GetStyle().PopupRounding = static_cast<float>(
              style["PopupRounding"].value<double>().value());
        }
        if (style["PopupBorderSize"]) {
          ImGui::GetStyle().PopupBorderSize = static_cast<float>(
              style["PopupBorderSize"].value<double>().value());
        }
        if (style["FramePadding"]) {
          auto vec2 = style["FramePadding"];
          ImGui::GetStyle().FramePadding = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["FrameRounding"]) {
          ImGui::GetStyle().FrameRounding = static_cast<float>(
              style["FrameRounding"].value<double>().value());
        }
        if (style["FrameBorderSize"]) {
          ImGui::GetStyle().FrameBorderSize = static_cast<float>(
              style["FrameBorderSize"].value<double>().value());
        }
        if (style["ItemSpacing"]) {
          auto vec2 = style["ItemSpacing"];
          ImGui::GetStyle().ItemSpacing = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["ItemInnerSpacing"]) {
          auto vec2 = style["ItemInnerSpacing"];
          ImGui::GetStyle().ItemInnerSpacing = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["TouchExtraPadding"]) {
          auto vec2 = style["TouchExtraPadding"];
          ImGui::GetStyle().TouchExtraPadding = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["IndentSpacing"]) {
          ImGui::GetStyle().IndentSpacing = static_cast<float>(
              style["IndentSpacing"].value<double>().value());
        }
        if (style["ColumnsMinSpacing"]) {
          ImGui::GetStyle().ColumnsMinSpacing = static_cast<float>(
              style["ColumnsMinSpacing"].value<double>().value());
        }
        if (style["ScrollbarSize"]) {
          ImGui::GetStyle().ScrollbarSize = static_cast<float>(
              style["ScrollbarSize"].value<double>().value());
        }
        if (style["ScrollbarRounding"]) {
          ImGui::GetStyle().ScrollbarRounding = static_cast<float>(
              style["ScrollbarRounding"].value<double>().value());
        }
        if (style["GrabMinSize"]) {
          ImGui::GetStyle().GrabMinSize =
              static_cast<float>(style["GrabMinSize"].value<double>().value());
        }
        if (style["GrabRounding"]) {
          ImGui::GetStyle().GrabRounding =
              static_cast<float>(style["GrabRounding"].value<double>().value());
        }
        if (style["ButtonTextAlign"]) {
          auto vec2 = style["ButtonTextAlign"];
          ImGui::GetStyle().ButtonTextAlign = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["DisplayWindowPadding"]) {
          auto vec2 = style["DisplayWindowPadding"];
          ImGui::GetStyle().DisplayWindowPadding = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["DisplaySafeAreaPadding"]) {
          auto vec2 = style["DisplaySafeAreaPadding"];
          ImGui::GetStyle().DisplaySafeAreaPadding = {
              static_cast<float>(vec2[0].value<double>().value()),
              static_cast<float>(vec2[1].value<double>().value())};
        }

        if (style["MouseCursorScale"]) {
          ImGui::GetStyle().MouseCursorScale = static_cast<float>(
              style["MouseCursorScale"].value<double>().value());
        }
        if (style["AntiAliasedLines"]) {
          ImGui::GetStyle().AntiAliasedLines =
              style["AntiAliasedLines"].as<bool>();
        }
        if (style["AntiAliasedFill"]) {
          ImGui::GetStyle().AntiAliasedFill =
              style["AntiAliasedFill"].as<bool>();
        }
        if (style["CurveTessellationTol"]) {
          ImGui::GetStyle().CurveTessellationTol = static_cast<float>(
              style["CurveTessellationTol"].value<double>().value());
        }
      }
      if (theme["colors"]) {
        auto &colors = ImGui::GetStyle().Colors;
        auto colors_settings = theme["colors"];

        SET_COLOR_FROM_TOML(ImGuiCol_Text);
        SET_COLOR_FROM_TOML(ImGuiCol_TextDisabled);
        SET_COLOR_FROM_TOML(ImGuiCol_WindowBg);
        SET_COLOR_FROM_TOML(ImGuiCol_ChildBg);
        SET_COLOR_FROM_TOML(ImGuiCol_PopupBg);
        SET_COLOR_FROM_TOML(ImGuiCol_Border);
        SET_COLOR_FROM_TOML(ImGuiCol_BorderShadow);
        SET_COLOR_FROM_TOML(ImGuiCol_FrameBg);
        SET_COLOR_FROM_TOML(ImGuiCol_FrameBgHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_FrameBgActive);
        SET_COLOR_FROM_TOML(ImGuiCol_TitleBg);
        SET_COLOR_FROM_TOML(ImGuiCol_TitleBgActive);
        SET_COLOR_FROM_TOML(ImGuiCol_TitleBgCollapsed);
        SET_COLOR_FROM_TOML(ImGuiCol_MenuBarBg);
        SET_COLOR_FROM_TOML(ImGuiCol_ScrollbarBg);
        SET_COLOR_FROM_TOML(ImGuiCol_ScrollbarGrab);
        SET_COLOR_FROM_TOML(ImGuiCol_ScrollbarGrabHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_ScrollbarGrabActive);
        SET_COLOR_FROM_TOML(ImGuiCol_CheckMark);
        SET_COLOR_FROM_TOML(ImGuiCol_SliderGrab);
        SET_COLOR_FROM_TOML(ImGuiCol_SliderGrabActive);
        SET_COLOR_FROM_TOML(ImGuiCol_Button);
        SET_COLOR_FROM_TOML(ImGuiCol_ButtonHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_ButtonActive);
        SET_COLOR_FROM_TOML(ImGuiCol_Header);
        SET_COLOR_FROM_TOML(ImGuiCol_HeaderHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_HeaderActive);
        SET_COLOR_FROM_TOML(ImGuiCol_Separator);
        SET_COLOR_FROM_TOML(ImGuiCol_SeparatorHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_SeparatorActive);
        SET_COLOR_FROM_TOML(ImGuiCol_ResizeGrip);
        SET_COLOR_FROM_TOML(ImGuiCol_ResizeGripHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_ResizeGripActive);
        SET_COLOR_FROM_TOML(ImGuiCol_PlotLines);
        SET_COLOR_FROM_TOML(ImGuiCol_PlotLinesHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_PlotHistogram);
        SET_COLOR_FROM_TOML(ImGuiCol_PlotHistogramHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_PlotHistogramHovered);
        SET_COLOR_FROM_TOML(ImGuiCol_TextSelectedBg);
        SET_COLOR_FROM_TOML(ImGuiCol_DragDropTarget);
        SET_COLOR_FROM_TOML(ImGuiCol_NavHighlight);
        SET_COLOR_FROM_TOML(ImGuiCol_NavWindowingHighlight);
        SET_COLOR_FROM_TOML(ImGuiCol_NavWindowingHighlight);
        SET_COLOR_FROM_TOML(ImGuiCol_NavWindowingDimBg);
        SET_COLOR_FROM_TOML(ImGuiCol_ModalWindowDimBg);
      }
    }
  } catch (std::exception const &ex) {
    ASLOG(error, "error {} while loading theme settings from {}", ex.what(),
        theme_settings.string());
    LoadDefaultStyle();
  }
}

} // namespace asap::ui
