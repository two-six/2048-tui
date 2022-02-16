#include <random>
#include "Board.h"
#include "ftxui/dom/elements.hpp"
#include <sys/types.h>
#include <sstream>
#include <string>
#include <array>

using namespace ftxui;

Board::Board(bool col) {
    colors = col;
    std::uniform_int_distribution<int> distribution(0, 15);
    engine.seed(std::random_device{}());
    auto draw_number = std::bind(distribution, engine);
    first_draw_number = draw_number();
    second_draw_number = draw_number();
    while(first_draw_number == second_draw_number)
        second_draw_number = draw_number();

    for(auto i=0; i < 16; ++i) {
        if(i == first_draw_number)
            cells[i] = 2;
        else if(i == second_draw_number)
            cells[i] = 2;
        else
            cells[i] = 0;

    }
}

auto Board::cell(u_int val) -> Element {
    std::string t;
    std::ostringstream tmp;
    tmp << val;
    t = tmp.str();
    if(colors) {
        auto cell = [](std::string t) { return text(t) | center | flex; };
        switch(val) {
            case 2:
                return cell(t) | bgcolor(Color::BlueLight);
            case 4:
                return cell(t) | bgcolor(Color::Blue);
            case 8:
                return cell(t) | bgcolor(Color::CyanLight);
            case 16:
                return cell(t) | bgcolor(Color::Cyan);
            case 32:
                return cell(t) | bgcolor(Color::GreenLight);
            case 64:
                return cell(t) | bgcolor(Color::Green);
            case 128:
                return cell(t) | bgcolor(Color::MagentaLight);
            case 256:
                return cell(t) | bgcolor(Color::Magenta);
            case 512:
                return cell(t) | bgcolor(Color::RedLight);
            case 1028:
                return cell(t) | bgcolor(Color::Red);
            case 2048:
                return cell(t) | bgcolor(Color::Red);
            default:
                return cell("");
        };
    } else {
        auto cell = [](std::string t) { return text(t) | center | border | flex; };
        if(val == 0)
            return cell("");
        else
            return cell(t);
    }
}

auto Board::move(u_short dir) -> void {
    enum d {
        left,
        down,
        right,
        up,
    };
    bool moved = false;

    std::array<bool, 16> cnt;
    for(auto i=0; i < 16; ++i)
        cnt[i] = 0;

    switch(dir) {
        case left:
            for(auto i=0; i < 4; ++i) {
                for(auto j=0; j < 3; ++j) {
                    short tmp = (j+1)+(4*i);
                    short bck = -1;
                    if(cells[tmp] == cells[tmp+bck] && cells[tmp] != 0 && !cnt[tmp+bck]) {
                        cells[tmp+bck] *= 2;
                        cells[tmp] = 0;
                        cnt[tmp+bck] = true;
                        moved = true;
                    } else if(cells[tmp+bck] == 0 && cells[tmp] != 0) {
                        cells[tmp+bck] = cells[tmp];
                        cells[tmp] = 0;
                        moved = true;
                        for(auto k=1; k < 3; ++k) {
                            auto nxt = tmp+(bck*(k+1));
                            if(nxt == 3 || nxt == 7 || nxt == 11)
                                break;
                            else if(cells[nxt] == cells[nxt-bck] && !cnt[nxt]) {
                                cells[nxt] *= 2;
                                cells[nxt-bck] = 0;
                                cnt[nxt] = true;
                            } else if(cells[nxt] == 0 && cells[nxt-bck] != 0) {
                                cells[nxt] = cells[nxt-bck];
                                cells[nxt-bck] = 0;
                            }
                        }
                    }
                }
            }
            break;
        case down:
            for(auto i=0; i < 4; ++i) {
                for(auto j=0; j < 3; ++j) {
                    short tmp = 8+i-(4*j);
                    short bck = 4;
                    if(cells[tmp] == cells[tmp+bck] && cells[tmp] != 0 && !cnt[tmp+bck]) {
                        cells[tmp+bck] *= 2;
                        cells[tmp] = 0;
                        moved = true;
                        cnt[tmp+bck] = true;
                    } else if(cells[tmp+bck] == 0 && cells[tmp] != 0) {
                        cells[tmp+bck] = cells[tmp];
                        cells[tmp] = 0;
                        moved = true;
                        for(auto k=1; k < 3; ++k) {
                            auto nxt = tmp+(bck*(k+1));
                            if(nxt > 15)
                                break;
                            else if(cells[nxt] == cells[nxt-bck] && !cnt[tmp+bck]) {
                                cells[nxt] *= 2;
                                cells[nxt-bck] = 0;
                                cnt[nxt] = true;
                            } else if(cells[nxt] == 0 && cells[nxt-bck] != 0) {
                                cells[nxt] = cells[nxt-bck];
                                cells[nxt-bck] = 0;
                            }
                        }
                    }
                }
            }
            break;
        case right:
            for(auto i=0; i < 4; ++i) {
                for(auto j=0; j < 3; ++j) {
                    short tmp = 2-j+(4*i);
                    short bck = 1;
                    if(cells[tmp] == cells[tmp+bck] && cells[tmp] != 0 && !cnt[tmp+bck]) {
                        cells[tmp+bck] *= 2;
                        cells[tmp] = 0;
                        moved = true;
                        cnt[tmp+bck] = true;
                    } else if(cells[tmp+bck] == 0 && cells[tmp] != 0) {
                        cells[tmp+bck] = cells[tmp];
                        cells[tmp] = 0;
                        moved = true;
                        for(auto k=1; k < 3; ++k) {
                            auto nxt = tmp+(bck*(k+1));
                            if(nxt == 4 || nxt == 8 || nxt == 12)
                                break;
                            else if(cells[nxt] == cells[nxt-bck] && !cnt[tmp+bck]) {
                                cells[nxt] *= 2;
                                cells[nxt-bck] = 0;
                                cnt[nxt] = true;
                            } else if(cells[nxt] == 0 && cells[nxt-bck] != 0) {
                                cells[nxt] = cells[nxt-bck];
                                cells[nxt-bck] = 0;
                            }
                        }
                    }
                }
            }
            break;
        case up:
            for(auto i=0; i < 4; ++i) {
                for(auto j=0; j < 3; ++j) {
                    short tmp = 4+i+(4*j);
                    short bck = -4;
                    if(cells[tmp] == cells[tmp+bck] && cells[tmp] != 0 && !cnt[tmp+bck]) {
                        cells[tmp+bck] *= 2;
                        cells[tmp] = 0;
                        moved = true;
                        cnt[tmp+bck] = true;
                    } else if(cells[tmp+bck] == 0 && cells[tmp] != 0) {
                        cells[tmp+bck] = cells[tmp];
                        cells[tmp] = 0;
                        moved = true;
                        for(auto k=1; k < 3; ++k) {
                            auto nxt = tmp+(bck*(k+1));
                            if(nxt < 0)
                                break;
                            else if(cells[nxt] == cells[nxt-bck] && !cnt[tmp+bck]) {
                                cells[nxt] *= 2;
                                cells[nxt-bck] = 0;
                                cnt[nxt] = true;
                            } else if(cells[nxt] == 0 && cells[nxt-bck] != 0) {
                                cells[nxt] = cells[nxt-bck];
                                cells[nxt-bck] = 0;
                            }
                        }
                    }
                }
            }
            break;
        default:
            return;
    };
    if(moved == false) {
        bool f = true;
        for(auto const i : cells) {
            if(i == 0) {
                f = false;
            }
        }
        full = f;

        return;
    }
    std::uniform_int_distribution<int> distribution(0, 15);
    engine.seed(std::random_device{}());
    auto draw_number = std::bind(distribution, engine);
    first_draw_number = draw_number();
    while(cells[first_draw_number] != 0) {
        first_draw_number = draw_number();
    }
    std::uniform_int_distribution<int> two_or_four(0, 1);
    auto draw_two_or_four = std::bind(two_or_four, engine);
    second_draw_number = draw_two_or_four();
    if(second_draw_number == 0)
        cells[first_draw_number] = 2;
    else
        cells[first_draw_number] = 4;
}

auto Board::check_if_full() -> bool {
    return full;
}

auto Board::check_if_won() -> bool {
    for(auto const i : cells) {
        if(i == 2048)
            return true;
    }
    return false;
}


auto Board::draw() -> Element {
    auto document =  //
        window(text("2048"), gridbox({
            {
                cell(cells[0]),
                cell(cells[1]),
                cell(cells[2]),
                cell(cells[3]),
            },
            {
                cell(cells[4]),
                cell(cells[5]),
                cell(cells[6]),
                cell(cells[7]),
            },
            {
                cell(cells[8]),
                cell(cells[9]),
                cell(cells[10]),
                cell(cells[11]),
            },
            {
                cell(cells[12]),
                cell(cells[13]),
                cell(cells[14]),
                cell(cells[15]),
            },
        }));
    return document;
}
