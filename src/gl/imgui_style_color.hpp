/*
 * imgui_style_color.hpp
 * 
 * 
 * Create Date : 2019-11-27 22:01:13
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef IMGUI_STYLE_COLOR_HPP__
#define IMGUI_STYLE_COLOR_HPP__

#include <string>
#include <vector>
#include <nameof.hpp>
#include <fmt/format.h>
#include "imgui.h"

namespace gl {

class ImGuiStyleColor {
  private:
    enum class Theme {
        // Theme を追加するときはここと、setCurrentTheme に追記する
        Dark,
        Classic,
        Light,
        End // guard
    };

  public:
    std::vector<std::string> getAllThemeNames() {
        std::vector<std::string> themes;
        for (int i = static_cast<int>(Theme::Dark); i < static_cast<int>(Theme::End); ++i) {
            themes.emplace_back(NAMEOF_ENUM(static_cast<Theme>(i)));
        }
        return themes;
    }

    Theme getCurrentTheme() { return current_theme; }
    void setCurrentTheme(std::string theme_name="Dark") {
        // TODO: refactor (Theme に begin(), end(), operator++ を実装して std::none_of を使う)
        bool is_valid_theme_name = false;
        for (int i = static_cast<int>(Theme::Dark); i < static_cast<int>(Theme::End); ++i) {
            if (NAMEOF_ENUM(static_cast<Theme>(i)) == theme_name) {
                current_theme = static_cast<Theme>(i);
                is_valid_theme_name = true;
                break;
            }
        }

        if (!is_valid_theme_name) {
            fmt::print("Invalid theme name : {}\n", theme_name);
            auto all_themes = getAllThemeNames();
            std::cout << "Select one of ";
            for (auto theme_name : all_themes) {
                std::cout << theme_name << ", ";
            }
        }

        switch (current_theme) {
        case Theme::Dark:
            ImGui::StyleColorsDark();
            break;
        case Theme::Classic:
            ImGui::StyleColorsClassic();
            break;
        case Theme::Light:
            ImGui::StyleColorsLight();
            break;
        default:
            ImGui::StyleColorsDark();
            break;
        }
    }

  private:
    Theme current_theme = Theme::Dark;
};

}

#endif // IMGUI_STYLE_COLOR_HPP__
