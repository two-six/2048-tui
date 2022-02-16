#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <memory>
#include <string>
#include "Board.h"

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"

using namespace ftxui;

auto arrow(Event event) -> u_short {
  std::string tmp;
  for(auto &it : event.input()) {
    tmp += std::to_string((u_int)it);
  }

  //left
  if(tmp == "279168")
    return 0;
  //down
  if(tmp == "279166")
    return 1;
  //right
  if(tmp == "279167")
    return 2;
  //up
  if(tmp == "279165")
    return 3;

  return 4;
}

auto main(int argc, const char* argv[]) -> int {
  bool colors = true;
  if(argc == 2) {
    if(std::string(argv[1]) == "--no-colors")
      colors = false;
    else {
      std::cout << "Wrong argument!\n";
      return 1;
    }
  } else if(argc > 2) {
    std::cout << "Wrong number of arguments!\n";
    return 2;
  }
  auto main_board = Board(colors);

  Event key;

  auto screen = ScreenInteractive::Fullscreen();

  auto component = Renderer([&] {
    if(main_board.check_if_won()) {
      return text("You Won!") | flex;
    }
    main_board.move(arrow(key));
    screen.Clear();
    return main_board.draw() | flex;
  });
  component = CatchEvent(component, [&](Event event) {
    key = event;
    return true;
  });

  screen.Loop(component);

  return 0;
}
