#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <sys/types.h>
#include <utility>
#include <random>
#include <ftxui/component/component.hpp>

class Board {
        std::array<u_int, 16> cells;
        std::mt19937 engine;
        u_short first_draw_number;
        u_short second_draw_number;
        auto cell(u_int) -> ftxui::Element;
        bool colors;
        bool full = false;
    public:
        Board(bool);
        auto move(u_short) -> void;
        auto check_if_full() -> bool;
        auto check_if_won() -> bool;
        auto draw() -> ftxui::Element;
};

#endif
