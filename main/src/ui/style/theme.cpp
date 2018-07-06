//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <array>
#include <cstring>
#include <map>
#include <mutex>  // for call_once()
#include <fstream>

#include <imgui/imgui.h>
#include <yaml-cpp/yaml.h>

#include <common/logging.h>
#include <ui/fonts/fonts.h>
#include <ui/fonts/material_design_icons.h>
#include <ui/style/theme.h>
#include <config.h>

namespace bfs = boost::filesystem;

namespace YAML {

YAML::Emitter &operator<<(YAML::Emitter &out, const ImVec4 &vec) {
  out << YAML::Flow;
  out << YAML::BeginSeq;
  out << vec.x << vec.y << vec.z << vec.w;
  out << YAML::EndSeq;
  return out;
}

}  // namespace YAML

namespace asap {
namespace ui {

std::string const Font::FAMILY_MONOSPACE{"Inconsolata"};
std::string const Font::FAMILY_PROPORTIONAL{"Roboto"};

std::map<std::string, ImFont *> Theme::fonts_;
ImFont *Theme::icons_font_normal_{nullptr};

namespace {

std::string BuildFontName(std::string const &family, Font::Weight weight,
                          Font::Style style, Font::Size size) {
  std::string name(family);
  name.append(" ").append(Font::WeightString(weight));
  if (style == Font::Style::ITALIC)
    name.append(" ").append(Font::StyleString(style));
  name.append(" ").append(Font::SizeString(size));
  return name;
}

/// Merge in icons from Font Material Design Icons font.
ImFont *MergeIcons(float size) {
  ImGuiIO &io = ImGui::GetIO();
  // The ranges array is not copied by the AddFont* functions and is used lazily
  // so ensure it is available for duration of font usage
  static const ImWchar icons_ranges[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};

  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  auto font = io.Fonts->AddFontFromMemoryCompressedTTF(
      asap::debug::ui::Fonts::MATERIAL_DESIGN_ICONS_COMPRESSED_DATA,
      asap::debug::ui::Fonts::MATERIAL_DESIGN_ICONS_COMPRESSED_SIZE, size,
      &icons_config, icons_ranges);
  // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

  return font;
}

ImFont *LoadRobotoFont(std::string const &name, Font::Weight weight,
                       Font::Style style, Font::Size size) {
  ImGuiIO &io = ImGui::GetIO();
  ImFontConfig fontConfig;
  fontConfig.MergeMode = false;
  std::strncpy(fontConfig.Name, name.c_str(), 40);
  ImFont *font = nullptr;
  switch (weight) {
    case Font::Weight::LIGHT:
      switch (style) {
        case Font::Style::ITALIC:
          io.Fonts->AddFontFromMemoryCompressedTTF(
              asap::debug::ui::Fonts::ROBOTO_LIGHTITALIC_COMPRESSED_DATA,
              asap::debug::ui::Fonts::ROBOTO_LIGHTITALIC_COMPRESSED_SIZE,
              Font::SizeFloat(size), &fontConfig,
              io.Fonts->GetGlyphRangesDefault());
          font = MergeIcons(Font::SizeFloat(size));
          break;
        case Font::Style::NORMAL:
          io.Fonts->AddFontFromMemoryCompressedTTF(
              asap::debug::ui::Fonts::ROBOTO_LIGHT_COMPRESSED_DATA,
              asap::debug::ui::Fonts::ROBOTO_LIGHT_COMPRESSED_SIZE,
              Font::SizeFloat(size), &fontConfig,
              io.Fonts->GetGlyphRangesDefault());
          font = MergeIcons(Font::SizeFloat(size));
          break;
      }
      break;
    case Font::Weight::REGULAR:
      switch (style) {
        case Font::Style::ITALIC:
          io.Fonts->AddFontFromMemoryCompressedTTF(
              asap::debug::ui::Fonts::ROBOTO_ITALIC_COMPRESSED_DATA,
              asap::debug::ui::Fonts::ROBOTO_ITALIC_COMPRESSED_SIZE,
              Font::SizeFloat(size), &fontConfig,
              io.Fonts->GetGlyphRangesDefault());
          font = MergeIcons(Font::SizeFloat(size));
          break;
        case Font::Style::NORMAL:
          io.Fonts->AddFontFromMemoryCompressedTTF(
              asap::debug::ui::Fonts::ROBOTO_REGULAR_COMPRESSED_DATA,
              asap::debug::ui::Fonts::ROBOTO_REGULAR_COMPRESSED_SIZE,
              Font::SizeFloat(size), &fontConfig,
              io.Fonts->GetGlyphRangesDefault());
          font = MergeIcons(Font::SizeFloat(size));
          break;
      }
      break;
    case Font::Weight::BOLD:
      switch (style) {
        case Font::Style::ITALIC:
          io.Fonts->AddFontFromMemoryCompressedTTF(
              asap::debug::ui::Fonts::ROBOTO_BOLDITALIC_COMPRESSED_DATA,
              asap::debug::ui::Fonts::ROBOTO_BOLDITALIC_COMPRESSED_SIZE,
              Font::SizeFloat(size), &fontConfig,
              io.Fonts->GetGlyphRangesDefault());
          font = MergeIcons(Font::SizeFloat(size));
          break;
        case Font::Style::NORMAL:
          io.Fonts->AddFontFromMemoryCompressedTTF(
              asap::debug::ui::Fonts::ROBOTO_BOLD_COMPRESSED_DATA,
              asap::debug::ui::Fonts::ROBOTO_BOLD_COMPRESSED_SIZE,
              Font::SizeFloat(size), &fontConfig);
          font = MergeIcons(Font::SizeFloat(size));
          break;
      }
      break;
  }
  return font;
}

ImFont *LoadInconsolataFont(std::string const &name, Font::Weight weight,
                            Font::Style style, Font::Size size) {
  ImGuiIO &io = ImGui::GetIO();
  ImFontConfig fontConfig;
  fontConfig.MergeMode = false;
  std::strncpy(fontConfig.Name, name.c_str(), 40);
  ImFont *font = nullptr;
  switch (weight) {
    case Font::Weight::LIGHT:
      break;
    case Font::Weight::REGULAR:
      switch (style) {
        case Font::Style::ITALIC:
        case Font::Style::NORMAL:
          io.Fonts->AddFontFromMemoryCompressedTTF(
              asap::debug::ui::Fonts::INCONSOLATA_REGULAR_COMPRESSED_DATA,
              asap::debug::ui::Fonts::INCONSOLATA_REGULAR_COMPRESSED_SIZE,
              Font::SizeFloat(size), &fontConfig);
          font = MergeIcons(Font::SizeFloat(size));
          break;
      }
      break;
    case Font::Weight::BOLD:
      switch (style) {
        case Font::Style::ITALIC:
        case Font::Style::NORMAL:
          io.Fonts->AddFontFromMemoryCompressedTTF(
              asap::debug::ui::Fonts::INCONSOLATA_BOLD_COMPRESSED_DATA,
              asap::debug::ui::Fonts::INCONSOLATA_BOLD_COMPRESSED_SIZE,
              Font::SizeFloat(size), &fontConfig);
          font = MergeIcons(Font::SizeFloat(size));
          break;
      }
      break;
  }
  return font;
}

ImFont *LoadIconsFont(float size) {
  ImGuiIO &io = ImGui::GetIO();
  ImFontConfig fontConfig;
  fontConfig.MergeMode = false;
  std::strncpy(fontConfig.Name, "Material Design Icons", 40);
  ImFont *font = nullptr;
  font = io.Fonts->AddFontFromMemoryCompressedTTF(
      asap::debug::ui::Fonts::MATERIAL_DESIGN_ICONS_COMPRESSED_DATA,
      asap::debug::ui::Fonts::MATERIAL_DESIGN_ICONS_COMPRESSED_SIZE, size,
      &fontConfig);
  return font;
}

}  // namespace

Font::Font(std::string family) : family_(std::move(family)) { InitFont(); }

void Font::InitFont() {
  BuildName();
  font_ = Theme::GetFont(name_);
  name_.assign(font_->GetDebugName());
}

Font::Font(Font const &other)
    : font_(other.font_),
      size_(other.size_),
      style_(other.style_),
      weight_(other.weight_),
      name_(other.name_) {}

Font &Font::operator=(Font const &rhs) {
  font_ = rhs.font_;
  size_ = rhs.size_;
  style_ = rhs.style_;
  weight_ = rhs.weight_;
  name_ = rhs.name_;
  return *this;
}
Font::Font(Font &&moved) noexcept
    : font_(moved.font_),
      size_(moved.size_),
      style_(moved.style_),
      weight_(moved.weight_),
      name_(std::move(moved.name_)) {
  moved.font_ = nullptr;
}
Font &Font::operator=(Font &&moved) noexcept {
  auto tmp = std::move(moved);
  swap(tmp);
  return *this;
}
void Font::swap(Font &other) {
  std::swap(font_, other.font_);
  std::swap(size_, other.size_);
  std::swap(style_, other.style_);
  std::swap(weight_, other.weight_);
  name_.swap(other.name_);
}

void Font::BuildName() {
  name_ = BuildFontName(family_, weight_, style_, size_);
}

Font &Font::SmallSize() {
  size_ = Size::SMALL;
  InitFont();
  return *this;
}
Font &Font::MediumSize() {
  size_ = Size::MEDIUM;
  InitFont();
  return *this;
}
Font &Font::LargeSize() {
  size_ = Size::LARGE;
  InitFont();
  return *this;
}
Font &Font::LargerSize() {
  size_ = Size::LARGER;
  InitFont();
  return *this;
}

Font &Font::Normal() {
  style_ = Style::NORMAL;
  InitFont();
  return *this;
}

Font &Font::Italic() {
  style_ = Style::ITALIC;
  InitFont();
  return *this;
}

Font &Font::Light() {
  weight_ = Weight::LIGHT;
  InitFont();
  return *this;
}
Font &Font::Regular() {
  weight_ = Weight::REGULAR;
  InitFont();
  return *this;
}
Font &Font::Bold() {
  weight_ = Weight::BOLD;
  InitFont();
  return *this;
}

float Font::SizeFloat(Font::Size size) {
  auto val = static_cast<typename std::underlying_type<Font::Size>::type>(size);
  return static_cast<float>(val);
}
char const *Font::SizeString(Font::Size size) {
  switch (size) {
    case Size::SMALL:
      return "11px";
    case Size::MEDIUM:
      return "13px";
    case Size::LARGE:
      return "16px";
    case Size::LARGER:
      return "24px";
  }
  // Only needed for compilers that complain about not all control paths
  // return a value.
  return "__NEVER__";
}

char const *Font::StyleString(Font::Style style) {
  switch (style) {
    case Style::NORMAL:
      return "Normal";
    case Style::ITALIC:
      return "Italic";
  }
  // Only needed for compilers that complain about not all control paths
  // return a value.
  return "__NEVER__";
}

char const *Font::WeightString(Font::Weight weight) {
  switch (weight) {
    case Weight::LIGHT:
      return "Light";
    case Weight::REGULAR:
      return "Regular";
    case Weight::BOLD:
      return "Bold";
  }
  // Only needed for compilers that complain about not all control paths
  // return a value.
  return "__NEVER__";
}

void Theme::Init() {
  LoadStyle();

  //
  // Fonts
  //
  static std::once_flag init_flag;
  std::call_once(init_flag, []() {
    ImGui::GetIO().Fonts->AddFontDefault();
    LoadDefaultFonts();
    // TODO: Temporary default font - can be configured
    Font default_font(Font::FAMILY_PROPORTIONAL);
    default_font.Regular().MediumSize();
    ImGui::GetIO().FontDefault = default_font.ImGuiFont();
  });
}

void Theme::LoadDefaultFonts() {
  std::array<Font::Weight, 3> font_weights{
      {Font::Weight::LIGHT, Font::Weight::REGULAR, Font::Weight::BOLD}};
  std::array<Font::Style, 2> font_styles{
      {Font::Style::NORMAL, Font::Style::ITALIC}};
  std::array<Font::Size, 4> font_sizes{{Font::Size::SMALL, Font::Size::MEDIUM,
                                        Font::Size::LARGE, Font::Size::LARGER}};
  for (auto size : font_sizes) {
    for (auto weight : font_weights) {
      for (auto style : font_styles) {
        auto name =
            BuildFontName(Font::FAMILY_PROPORTIONAL, weight, style, size);
        auto font = LoadRobotoFont(name, weight, style, size);
        if (font) AddFont(name, font);
      }
    }

    // Monospaced
    auto name = BuildFontName(Font::FAMILY_MONOSPACE, Font::Weight::REGULAR,
                              Font::Style::NORMAL, size);
    auto font = LoadInconsolataFont(name, Font::Weight::REGULAR,
                                    Font::Style::NORMAL, size);
    if (font) {
      AddFont(name, font);
      // No Italic
      AddFont(BuildFontName(Font::FAMILY_MONOSPACE, Font::Weight::REGULAR,
                            Font::Style::ITALIC, size),
              font);
      // Treat LIGHT same as REGULAR
      AddFont(BuildFontName(Font::FAMILY_MONOSPACE, Font::Weight::LIGHT,
                            Font::Style::NORMAL, size),
              font);
      AddFont(BuildFontName(Font::FAMILY_MONOSPACE, Font::Weight::LIGHT,
                            Font::Style::ITALIC, size),
              font);
    }

    name = BuildFontName(Font::FAMILY_MONOSPACE, Font::Weight::BOLD,
                         Font::Style::NORMAL, size);
    font = LoadInconsolataFont(name, Font::Weight::BOLD, Font::Style::NORMAL,
                               size);
    if (font) {
      AddFont(name, font);
      // No Italic
      AddFont(BuildFontName(Font::FAMILY_MONOSPACE, Font::Weight::BOLD,
                            Font::Style::ITALIC, size),
              font);
    }
  }

  // The Icons font
  icons_font_normal_ = LoadIconsFont(32.0f);
}

void Theme::LoadDefaultStyle() {
  auto &style = ImGui::GetStyle();

  style.WindowPadding = ImVec2(5, 5);
  style.WindowRounding = 0.0f;
  style.FramePadding = ImVec2(5, 5);
  style.FrameRounding = 3.0f;
  style.ItemSpacing = ImVec2(12, 5);
  style.ItemInnerSpacing = ImVec2(5, 5);
  style.IndentSpacing = 25.0f;
  style.ScrollbarSize = 15.0f;
  style.ScrollbarRounding = 9.0f;
  style.GrabMinSize = 5.0f;
  style.GrabRounding = 3.0f;

  // clang-format off
  style.Colors[ImGuiCol_Text] = ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.42f, 0.02f, 0.40f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.42f, 0.02f, 0.67f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 0.80f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.75f, 0.42f, 0.02f, 1.00f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.75f, 0.42f, 0.02f, 0.78f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.75f, 0.42f, 0.02f, 1.00f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.75f, 0.42f, 0.02f, 0.40f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.75f, 0.42f, 0.02f, 1.00f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.94f, 0.47f, 0.02f, 1.00f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.75f, 0.42f, 0.02f, 0.31f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.42f, 0.02f, 0.80f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.75f, 0.42f, 0.02f, 1.00f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.42f, 0.02f, 0.78f);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.42f, 0.02f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.75f, 0.42f, 0.02f, 0.67f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.75f, 0.42f, 0.02f, 0.95f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.57f, 0.65f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.10f, 0.30f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.40f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.75f, 0.42f, 0.02f, 0.35f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
  style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.06f, 0.06f, 0.06f, 0.35f);
  // clang-format on
}

ImFont *Theme::GetFont(std::string const &name) {
  auto font = fonts_.find(name);
  if (font == fonts_.end()) {
    return ImGui::GetFont();
  }
  return font->second;
}

void Theme::AddFont(std::string const &name, ImFont *font) {
  fonts_.insert({name, font});
}


// -------------------------------------------------------------------------
// Settings load/save
// -------------------------------------------------------------------------

namespace {
void ConfigSanityChecks(YAML::Node &config) {
  auto &logger = asap::logging::Registry::GetLogger(asap::logging::Id::MAIN);

  auto theme = config["theme"];
  if ( !theme) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'theme' in config");
  }

  if ( !theme["style"]) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'theme/style' in config");
  }

  if ( !theme["colors"]) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'theme/colors' in config");
  }
}

void SetStyleColor(char const *id_str, int id, YAML::Node const& root) {
  auto &colors = ImGui::GetStyle().Colors;
  if (root[id_str]) {
    auto color = root[id_str];
    colors[id] = {
        color[0].as<float>(),
        color[1].as<float>(),
        color[2].as<float>(),
        color[3].as<float>()
    };
  }
}

}  // namespace

void Theme::SaveStyle() {
  // clang-format off
  // begin generated code
  auto &style = ImGui::GetStyle();
  YAML::Emitter out;
  out << YAML::BeginMap;
  {
    out << YAML::Key << "theme";
    out << YAML::BeginMap;
    {
      out << YAML::Key << "style";
      out << YAML::BeginMap;
      {
        out << YAML::Key << "Alpha";
        out << YAML::Value << style.Alpha;
        out << YAML::Comment("Global alpha applies to everything in ImGui.");
        out << YAML::Key << "WindowPadding";
        out << YAML::Flow << YAML::BeginSeq << style.WindowPadding.x << style.WindowPadding.y << YAML::EndSeq;
        out << YAML::Comment("Padding within a window.");
        out << YAML::Key << "WindowRounding";
        out << YAML::Value << style.WindowRounding;
        out << YAML::Comment("Radius of window corners rounding. Set to 0.0f to have rectangular windows.");
        out << YAML::Key << "WindowBorderSize";
        out << YAML::Value << style.WindowBorderSize;
        out << YAML::Comment("Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).");
        out << YAML::Key << "WindowMinSize";
        out << YAML::Flow << YAML::BeginSeq << style.WindowMinSize.x << style.WindowMinSize.y << YAML::EndSeq;
        out << YAML::Comment("Minimum window size. This is a global setting. If you want to constraint individual windows, use SetNextWindowSizeConstraints().");
        out << YAML::Key << "WindowTitleAlign";
        out << YAML::Flow << YAML::BeginSeq << style.WindowTitleAlign.x << style.WindowTitleAlign.y << YAML::EndSeq;
        out << YAML::Comment("Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.");
        out << YAML::Key << "ChildRounding";
        out << YAML::Value << style.ChildRounding;
        out << YAML::Comment("Radius of child window corners rounding. Set to 0.0f to have rectangular windows.");
        out << YAML::Key << "ChildBorderSize";
        out << YAML::Value << style.ChildBorderSize;
        out << YAML::Comment("Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).");
        out << YAML::Key << "PopupRounding";
        out << YAML::Value << style.PopupRounding;
        out << YAML::Comment("Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)");
        out << YAML::Key << "PopupBorderSize";
        out << YAML::Value << style.PopupBorderSize;
        out << YAML::Comment("Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).");
        out << YAML::Key << "FramePadding";
        out << YAML::Flow << YAML::BeginSeq << style.FramePadding.x << style.FramePadding.y << YAML::EndSeq;
        out << YAML::Comment("Padding within a framed rectangle (used by most widgets).");
        out << YAML::Key << "FrameRounding";
        out << YAML::Value << style.FrameRounding;
        out << YAML::Comment("Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).");
        out << YAML::Key << "FrameBorderSize";
        out << YAML::Value << style.FrameBorderSize;
        out << YAML::Comment("Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).");
        out << YAML::Key << "ItemSpacing";
        out << YAML::Flow << YAML::BeginSeq << style.ItemSpacing.x << style.ItemSpacing.y << YAML::EndSeq;
        out << YAML::Comment("Horizontal and vertical spacing between widgets/lines.");
        out << YAML::Key << "ItemInnerSpacing";
        out << YAML::Flow << YAML::BeginSeq << style.ItemInnerSpacing.x << style.ItemInnerSpacing.y << YAML::EndSeq;
        out << YAML::Comment("Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).");
        out << YAML::Key << "TouchExtraPadding";
        out << YAML::Flow << YAML::BeginSeq << style.TouchExtraPadding.x << style.TouchExtraPadding.y << YAML::EndSeq;
        out << YAML::Comment("Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!");
        out << YAML::Key << "IndentSpacing";
        out << YAML::Value << style.IndentSpacing;
        out << YAML::Comment("Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).");
        out << YAML::Key << "ColumnsMinSpacing";
        out << YAML::Value << style.ColumnsMinSpacing;
        out << YAML::Comment("Minimum horizontal spacing between two columns.");
        out << YAML::Key << "ScrollbarSize";
        out << YAML::Value << style.ScrollbarSize;
        out << YAML::Comment("Width of the vertical scrollbar, Height of the horizontal scrollbar.");
        out << YAML::Key << "ScrollbarRounding";
        out << YAML::Value << style.ScrollbarRounding;
        out << YAML::Comment("Radius of grab corners for scrollbar.");
        out << YAML::Key << "GrabMinSize";
        out << YAML::Value << style.GrabMinSize;
        out << YAML::Comment("Minimum width/height of a grab box for slider/scrollbar.");
        out << YAML::Key << "GrabRounding";
        out << YAML::Value << style.GrabRounding;
        out << YAML::Comment("Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.");
        out << YAML::Key << "ButtonTextAlign";
        out << YAML::Flow << YAML::BeginSeq << style.ButtonTextAlign.x << style.ButtonTextAlign.y << YAML::EndSeq;
        out << YAML::Comment("Alignment of button text when button is larger than text. Defaults to (0.5f,0.5f) for horizontally+vertically centered.");
        out << YAML::Key << "DisplayWindowPadding";
        out << YAML::Flow << YAML::BeginSeq << style.DisplayWindowPadding.x << style.DisplayWindowPadding.y << YAML::EndSeq;
        out << YAML::Comment("Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.");
        out << YAML::Key << "DisplaySafeAreaPadding";
        out << YAML::Flow << YAML::BeginSeq << style.DisplaySafeAreaPadding.x << style.DisplaySafeAreaPadding.y << YAML::EndSeq;
        out << YAML::Comment("If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!");
        out << YAML::Key << "MouseCursorScale";
        out << YAML::Value << style.MouseCursorScale;
        out << YAML::Comment("Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.");
        out << YAML::Key << "AntiAliasedLines";
        out << YAML::Value << style.AntiAliasedLines;
        out << YAML::Comment("Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.");
        out << YAML::Key << "AntiAliasedFill";
        out << YAML::Value << style.AntiAliasedFill;
        out << YAML::Comment("Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)");
      }
      out << YAML::EndMap;
      // end generated code
      // clang-format on

      out << YAML::Newline;
      out << YAML::Newline;

      out << YAML::Key << "colors";
      out << YAML::BeginMap;
      {
        auto &colors = style.Colors;
        // clang-format off
        // begin generated code
        out << YAML::Key << "ImGuiCol_Text";
        out << YAML::Value << colors[ImGuiCol_Text];
        out << YAML::Key << "ImGuiCol_TextDisabled";
        out << YAML::Value << colors[ImGuiCol_TextDisabled];
        out << YAML::Key << "ImGuiCol_WindowBg";
        out << YAML::Value << colors[ImGuiCol_WindowBg];
        out << YAML::Comment("Background of normal windows");
        out << YAML::Key << "ImGuiCol_ChildBg";
        out << YAML::Value << colors[ImGuiCol_ChildBg];
        out << YAML::Comment("Background of child windows");
        out << YAML::Key << "ImGuiCol_PopupBg";
        out << YAML::Value << colors[ImGuiCol_PopupBg];
        out << YAML::Comment("Background of popups, menus, tooltips windows");
        out << YAML::Key << "ImGuiCol_Border";
        out << YAML::Value << colors[ImGuiCol_Border];
        out << YAML::Key << "ImGuiCol_BorderShadow";
        out << YAML::Value << colors[ImGuiCol_BorderShadow];
        out << YAML::Key << "ImGuiCol_FrameBg";
        out << YAML::Value << colors[ImGuiCol_FrameBg];
        out << YAML::Comment("Background of checkbox, radio button, plot, slider, text input");
        out << YAML::Key << "ImGuiCol_FrameBgHovered";
        out << YAML::Value << colors[ImGuiCol_FrameBgHovered];
        out << YAML::Key << "ImGuiCol_FrameBgActive";
        out << YAML::Value << colors[ImGuiCol_FrameBgActive];
        out << YAML::Key << "ImGuiCol_TitleBg";
        out << YAML::Value << colors[ImGuiCol_TitleBg];
        out << YAML::Key << "ImGuiCol_TitleBgActive";
        out << YAML::Value << colors[ImGuiCol_TitleBgActive];
        out << YAML::Key << "ImGuiCol_TitleBgCollapsed";
        out << YAML::Value << colors[ImGuiCol_TitleBgCollapsed];
        out << YAML::Key << "ImGuiCol_MenuBarBg";
        out << YAML::Value << colors[ImGuiCol_MenuBarBg];
        out << YAML::Key << "ImGuiCol_ScrollbarBg";
        out << YAML::Value << colors[ImGuiCol_ScrollbarBg];
        out << YAML::Key << "ImGuiCol_ScrollbarGrab";
        out << YAML::Value << colors[ImGuiCol_ScrollbarGrab];
        out << YAML::Key << "ImGuiCol_ScrollbarGrabHovered";
        out << YAML::Value << colors[ImGuiCol_ScrollbarGrabHovered];
        out << YAML::Key << "ImGuiCol_ScrollbarGrabActive";
        out << YAML::Value << colors[ImGuiCol_ScrollbarGrabActive];
        out << YAML::Key << "ImGuiCol_CheckMark";
        out << YAML::Value << colors[ImGuiCol_CheckMark];
        out << YAML::Key << "ImGuiCol_SliderGrab";
        out << YAML::Value << colors[ImGuiCol_SliderGrab];
        out << YAML::Key << "ImGuiCol_SliderGrabActive";
        out << YAML::Value << colors[ImGuiCol_SliderGrabActive];
        out << YAML::Key << "ImGuiCol_Button";
        out << YAML::Value << colors[ImGuiCol_Button];
        out << YAML::Key << "ImGuiCol_ButtonHovered";
        out << YAML::Value << colors[ImGuiCol_ButtonHovered];
        out << YAML::Key << "ImGuiCol_ButtonActive";
        out << YAML::Value << colors[ImGuiCol_ButtonActive];
        out << YAML::Key << "ImGuiCol_Header";
        out << YAML::Value << colors[ImGuiCol_Header];
        out << YAML::Key << "ImGuiCol_HeaderHovered";
        out << YAML::Value << colors[ImGuiCol_HeaderHovered];
        out << YAML::Key << "ImGuiCol_HeaderActive";
        out << YAML::Value << colors[ImGuiCol_HeaderActive];
        out << YAML::Key << "ImGuiCol_Separator";
        out << YAML::Value << colors[ImGuiCol_Separator];
        out << YAML::Key << "ImGuiCol_SeparatorHovered";
        out << YAML::Value << colors[ImGuiCol_SeparatorHovered];
        out << YAML::Key << "ImGuiCol_SeparatorActive";
        out << YAML::Value << colors[ImGuiCol_SeparatorActive];
        out << YAML::Key << "ImGuiCol_ResizeGrip";
        out << YAML::Value << colors[ImGuiCol_ResizeGrip];
        out << YAML::Key << "ImGuiCol_ResizeGripHovered";
        out << YAML::Value << colors[ImGuiCol_ResizeGripHovered];
        out << YAML::Key << "ImGuiCol_ResizeGripActive";
        out << YAML::Value << colors[ImGuiCol_ResizeGripActive];
        out << YAML::Key << "ImGuiCol_PlotLines";
        out << YAML::Value << colors[ImGuiCol_PlotLines];
        out << YAML::Key << "ImGuiCol_PlotLinesHovered";
        out << YAML::Value << colors[ImGuiCol_PlotLinesHovered];
        out << YAML::Key << "ImGuiCol_PlotHistogram";
        out << YAML::Value << colors[ImGuiCol_PlotHistogram];
        out << YAML::Key << "ImGuiCol_PlotHistogramHovered";
        out << YAML::Value << colors[ImGuiCol_PlotHistogramHovered];
        out << YAML::Key << "ImGuiCol_TextSelectedBg";
        out << YAML::Value << colors[ImGuiCol_TextSelectedBg];
        out << YAML::Key << "ImGuiCol_ModalWindowDarkening";
        out << YAML::Value << colors[ImGuiCol_ModalWindowDarkening];
        out << YAML::Comment("Darken/colorize entire screen behind a modal window, when one is active");
        out << YAML::Key << "ImGuiCol_DragDropTarget";
        out << YAML::Value << colors[ImGuiCol_DragDropTarget];
        out << YAML::Key << "ImGuiCol_NavHighlight";
        out << YAML::Value << colors[ImGuiCol_NavHighlight];
        out << YAML::Comment("Gamepad/keyboard: current highlighted item");
        out << YAML::Key << "ImGuiCol_NavWindowingHighlight";
        out << YAML::Value << colors[ImGuiCol_NavWindowingHighlight];
        out << YAML::Comment("Gamepad/keyboard: when holding NavMenu to focus/move/resize windows");
        // begin generated code
        // clang-format on
      }
      out << YAML::EndMap;
    }
    out << YAML::EndMap;
  }
  out << YAML::EndMap;

  auto theme_settings =
      asap::fs::GetPathFor(asap::fs::Location::F_THEME_SETTINGS);
  auto ofs = std::ofstream();
  ofs.open(theme_settings.string());
  ofs << out.c_str() << std::endl;
  ofs.close();
}

void Theme::LoadStyle() {
  auto &logger = asap::logging::Registry::GetLogger(asap::logging::Id::MAIN);

  YAML::Node config;
  auto theme_settings =
      asap::fs::GetPathFor(asap::fs::Location::F_THEME_SETTINGS);
  auto has_config = false;
  if (bfs::exists(theme_settings)) {
    try {
      config = YAML::LoadFile(theme_settings.string());
      ASLOG_TO_LOGGER(logger, info, "theme settings loaded from {}",
            theme_settings);
      has_config = true;
    } catch (std::exception const &ex) {
      ASLOG_TO_LOGGER(logger, error,
            "error () while loading theme settings from {}", ex.what(),
            theme_settings);
    }
  } else {
    ASLOG_TO_LOGGER(logger, info, "file {} does not exist", theme_settings);
  }

  if (has_config) {
    ConfigSanityChecks(config);

    if (config["theme"]) {
      if (config["theme"]["style"]) {
        auto style = config["theme"]["style"];
        // clang-format off
        // begin generated code
        if (style["Alpha"]) {
          ImGui::GetStyle().Alpha = style["Alpha"].as<float>();
        }
        if (style["WindowPadding"]) {
          ImGui::GetStyle().WindowPadding = {
              style["WindowPadding"][0].as<float>(),
              style["WindowPadding"][1].as<float>()
          };
        }
        if (style["WindowRounding"]) {
          ImGui::GetStyle().WindowRounding =
              style["WindowRounding"].as<float>();
        }
        if (style["WindowBorderSize"]) {
          ImGui::GetStyle().WindowBorderSize =
              style["WindowBorderSize"].as<float>();
        }
        if (style["WindowMinSize"]) {
          ImGui::GetStyle().WindowMinSize = {
              style["WindowMinSize"][0].as<float>(),
              style["WindowMinSize"][1].as<float>()
          };
        }
        if (style["WindowTitleAlign"]) {
          ImGui::GetStyle().WindowTitleAlign = {
              style["WindowTitleAlign"][0].as<float>(),
              style["WindowTitleAlign"][1].as<float>()
          };
        }
        if (style["ChildRounding"]) {
          ImGui::GetStyle().ChildRounding = style["ChildRounding"].as<float>();
        }
        if (style["ChildBorderSize"]) {
          ImGui::GetStyle().ChildBorderSize =
              style["ChildBorderSize"].as<float>();
        }
        if (style["PopupRounding"]) {
          ImGui::GetStyle().PopupRounding = style["PopupRounding"].as<float>();
        }
        if (style["PopupBorderSize"]) {
          ImGui::GetStyle().PopupBorderSize =
              style["PopupBorderSize"].as<float>();
        }
        if (style["FramePadding"]) {
          ImGui::GetStyle().FramePadding = {
              style["FramePadding"][0].as<float>(),
              style["FramePadding"][1].as<float>()
          };
        }
        if (style["FrameRounding"]) {
          ImGui::GetStyle().FrameRounding = style["FrameRounding"].as<float>();
        }
        if (style["FrameBorderSize"]) {
          ImGui::GetStyle().FrameBorderSize =
              style["FrameBorderSize"].as<float>();
        }
        if (style["ItemSpacing"]) {
          ImGui::GetStyle().ItemSpacing = {
              style["ItemSpacing"][0].as<float>(),
              style["ItemSpacing"][1].as<float>()
          };
        }
        if (style["ItemInnerSpacing"]) {
          ImGui::GetStyle().ItemInnerSpacing = {
              style["ItemInnerSpacing"][0].as<float>(),
              style["ItemInnerSpacing"][1].as<float>()
          };
        }
        if (style["TouchExtraPadding"]) {
          ImGui::GetStyle().TouchExtraPadding = {
              style["TouchExtraPadding"][0].as<float>(),
              style["TouchExtraPadding"][1].as<float>()
          };
        }
        if (style["IndentSpacing"]) {
          ImGui::GetStyle().IndentSpacing = style["IndentSpacing"].as<float>();
        }
        if (style["ColumnsMinSpacing"]) {
          ImGui::GetStyle().ColumnsMinSpacing =
              style["ColumnsMinSpacing"].as<float>();
        }
        if (style["ScrollbarSize"]) {
          ImGui::GetStyle().ScrollbarSize = style["ScrollbarSize"].as<float>();
        }
        if (style["ScrollbarRounding"]) {
          ImGui::GetStyle().ScrollbarRounding =
              style["ScrollbarRounding"].as<float>();
        }
        if (style["GrabMinSize"]) {
          ImGui::GetStyle().GrabMinSize = style["GrabMinSize"].as<float>();
        }
        if (style["GrabRounding"]) {
          ImGui::GetStyle().GrabRounding = style["GrabRounding"].as<float>();
        }
        if (style["ButtonTextAlign"]) {
          ImGui::GetStyle().ButtonTextAlign = {
              style["ButtonTextAlign"][0].as<float>(),
              style["ButtonTextAlign"][1].as<float>()
          };
        }
        if (style["DisplayWindowPadding"]) {
          ImGui::GetStyle().DisplayWindowPadding = {
              style["DisplayWindowPadding"][0].as<float>(),
              style["DisplayWindowPadding"][1].as<float>()
          };
        }
        if (style["DisplaySafeAreaPadding"]) {
          ImGui::GetStyle().DisplaySafeAreaPadding = {
              style["DisplaySafeAreaPadding"][0].as<float>(),
              style["DisplaySafeAreaPadding"][1].as<float>()
          };
        }
        if (style["MouseCursorScale"]) {
          ImGui::GetStyle().MouseCursorScale =
              style["MouseCursorScale"].as<float>();
        }
        if (style["AntiAliasedLines"]) {
          ImGui::GetStyle().AntiAliasedLines =
              style["AntiAliasedLines"].as<bool>();
        }
        if (style["AntiAliasedFill"]) {
          ImGui::GetStyle().AntiAliasedFill =
              style["AntiAliasedFill"].as<bool>();
        }
        // begin generated code
        // clang-format on
      }
      if (config["theme"]["colors"]) {
        auto colors = config["theme"]["colors"];
        // clang-format off
        // begin generated code
        SetStyleColor("ImGuiCol_Text", ImGuiCol_Text, colors);
        SetStyleColor("ImGuiCol_TextDisabled", ImGuiCol_TextDisabled, colors);
        SetStyleColor("ImGuiCol_WindowBg", ImGuiCol_WindowBg, colors);
        SetStyleColor("ImGuiCol_ChildBg", ImGuiCol_ChildBg, colors);
        SetStyleColor("ImGuiCol_PopupBg", ImGuiCol_PopupBg, colors);
        SetStyleColor("ImGuiCol_Border", ImGuiCol_Border, colors);
        SetStyleColor("ImGuiCol_BorderShadow", ImGuiCol_BorderShadow, colors);
        SetStyleColor("ImGuiCol_FrameBg", ImGuiCol_FrameBg, colors);
        SetStyleColor("ImGuiCol_FrameBgHovered", ImGuiCol_FrameBgHovered, colors);
        SetStyleColor("ImGuiCol_FrameBgActive", ImGuiCol_FrameBgActive, colors);
        SetStyleColor("ImGuiCol_TitleBg", ImGuiCol_TitleBg, colors);
        SetStyleColor("ImGuiCol_TitleBgActive", ImGuiCol_TitleBgActive, colors);
        SetStyleColor("ImGuiCol_TitleBgCollapsed", ImGuiCol_TitleBgCollapsed, colors);
        SetStyleColor("ImGuiCol_MenuBarBg", ImGuiCol_MenuBarBg, colors);
        SetStyleColor("ImGuiCol_ScrollbarBg", ImGuiCol_ScrollbarBg, colors);
        SetStyleColor("ImGuiCol_ScrollbarGrab", ImGuiCol_ScrollbarGrab, colors);
        SetStyleColor("ImGuiCol_ScrollbarGrabHovered", ImGuiCol_ScrollbarGrabHovered, colors);
        SetStyleColor("ImGuiCol_ScrollbarGrabActive", ImGuiCol_ScrollbarGrabActive, colors);
        SetStyleColor("ImGuiCol_CheckMark", ImGuiCol_CheckMark, colors);
        SetStyleColor("ImGuiCol_SliderGrab", ImGuiCol_SliderGrab, colors);
        SetStyleColor("ImGuiCol_SliderGrabActive", ImGuiCol_SliderGrabActive, colors);
        SetStyleColor("ImGuiCol_Button", ImGuiCol_Button, colors);
        SetStyleColor("ImGuiCol_ButtonHovered", ImGuiCol_ButtonHovered, colors);
        SetStyleColor("ImGuiCol_ButtonActive", ImGuiCol_ButtonActive, colors);
        SetStyleColor("ImGuiCol_Header", ImGuiCol_Header, colors);
        SetStyleColor("ImGuiCol_HeaderHovered", ImGuiCol_HeaderHovered, colors);
        SetStyleColor("ImGuiCol_HeaderActive", ImGuiCol_HeaderActive, colors);
        SetStyleColor("ImGuiCol_Separator", ImGuiCol_Separator, colors);
        SetStyleColor("ImGuiCol_SeparatorHovered", ImGuiCol_SeparatorHovered, colors);
        SetStyleColor("ImGuiCol_SeparatorActive", ImGuiCol_SeparatorActive, colors);
        SetStyleColor("ImGuiCol_ResizeGrip", ImGuiCol_ResizeGrip, colors);
        SetStyleColor("ImGuiCol_ResizeGripHovered", ImGuiCol_ResizeGripHovered, colors);
        SetStyleColor("ImGuiCol_ResizeGripActive", ImGuiCol_ResizeGripActive, colors);
        SetStyleColor("ImGuiCol_PlotLines", ImGuiCol_PlotLines, colors);
        SetStyleColor("ImGuiCol_PlotLinesHovered", ImGuiCol_PlotLinesHovered, colors);
        SetStyleColor("ImGuiCol_PlotHistogram", ImGuiCol_PlotHistogram, colors);
        SetStyleColor("ImGuiCol_PlotHistogramHovered", ImGuiCol_PlotHistogramHovered, colors);
        SetStyleColor("ImGuiCol_TextSelectedBg", ImGuiCol_TextSelectedBg, colors);
        SetStyleColor("ImGuiCol_ModalWindowDarkening", ImGuiCol_ModalWindowDarkening, colors);
        SetStyleColor("ImGuiCol_DragDropTarget", ImGuiCol_DragDropTarget, colors);
        SetStyleColor("ImGuiCol_NavHighlight", ImGuiCol_NavHighlight, colors);
        SetStyleColor("ImGuiCol_NavWindowingHighlight", ImGuiCol_NavWindowingHighlight, colors);        // begin generated code
        // clang-format on
      }
    }
  } else {
    LoadDefaultStyle();
  }
}

}  // namespace ui
}  // namespace asap
