#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

void tui(Settings Curr_Settings){
  Settings Old_Settings = Curr_Settings;

  std::vector<std::string> filter = {
    "Fast Roll Off Low Latency",
    "Fast Roll Off Phase Compensated",
    "Slow Roll Off Low Latency",
    "Slow Roll Off Phase Compensated",
    "Non Oversampling"
  };

  std::vector<std::string> gain = {"low", "high"};

  std::vector<std::string> indicator = {"on", "temp off", "off"};

  auto screen = ftxui::ScreenInteractive::TerminalOutput();

  auto filter_radiobox = ftxui::Radiobox(
      &filter,
      &Curr_Settings.filter_state,
      ftxui::RadioboxOption{
        .on_change = [&] {
        if (Old_Settings.filter_state != Curr_Settings.filter_state){
        set_filter(Curr_Settings.dac, &Curr_Settings.filter_state);
        Old_Settings.filter_state = Curr_Settings.filter_state;
        };
        }
      });

  auto gain_radiobox = ftxui::Radiobox(
      &gain,
      &Curr_Settings.gain_state,
      ftxui::RadioboxOption{
        .on_change = [&] {
        if (Old_Settings.gain_state != Curr_Settings.gain_state){
        set_gain(Curr_Settings.dac, &Curr_Settings.gain_state);
        Old_Settings.gain_state = Curr_Settings.gain_state;
        };
        }
      });

  auto indicator_radiobox = ftxui::Radiobox(
      &indicator,
      &Curr_Settings.indicator_state,
      ftxui::RadioboxOption{
        .on_change = [&] {
        if (Old_Settings.indicator_state != Curr_Settings.indicator_state){
        set_indicator(Curr_Settings.dac, &Curr_Settings.indicator_state);
        Old_Settings.indicator_state = Curr_Settings.indicator_state;
        };
        }
      }
      );

  auto volume_slider = ftxui::Slider(
      "volume: ",
      &Curr_Settings.volume,
      0,
      60,
      1
      );

  auto refresh_button = ftxui::Button(
    "refresh (r)",
    [&] { refresh_data(&Curr_Settings); }
    );

  auto apply_button = ftxui::Button(
    "apply volume (a)",
    [&] { set_volume(Curr_Settings.dac, &Curr_Settings.volume); }
    );

    auto container = ftxui::Container::Vertical({
      filter_radiobox,
      gain_radiobox,
      indicator_radiobox,
      volume_slider,
      apply_button,
      refresh_button
    });

  auto renderer = ftxui::Renderer(container, [&] {
      return ftxui::vbox(
          ftxui::text("Filter:"),
          filter_radiobox->Render(),
          ftxui::separator(),
          ftxui::text("Gain:"),
          gain_radiobox->Render(),
          ftxui::separator(),
          ftxui::text("Indicator:"),
          indicator_radiobox->Render(),
          ftxui::separator(),
          ftxui::hbox(
          volume_slider->Render(),
          ftxui::text(" "),
          ftxui::text(std::to_string(Curr_Settings.volume))),
          ftxui::separator(),
          ftxui::hbox(
          apply_button->Render(),
          refresh_button->Render())
          ) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 120);
      });

  auto quit_event = ftxui::CatchEvent(
    renderer,
    [&](ftxui::Event event) {
      if (event == ftxui::Event::Character('q')) {
        Curr_Settings.volume++;
        screen.ExitLoopClosure()();
        return true;
      }
    return false;
    }
  );

auto refresh_event = ftxui::CatchEvent(
    renderer,
    [&] (ftxui::Event event) {
      if (event == ftxui::Event::Character('r')) {
        refresh_data(&Curr_Settings);
        Curr_Settings.volume++;
        return true;
      }
    return false;
    }
  );

auto apply_volume_event = ftxui::CatchEvent(
    renderer,
    [&] (ftxui::Event event) {
      if (event == ftxui::Event::Character('a')) {
        set_volume(Curr_Settings.dac, &Curr_Settings.volume);  
        return true;
      }
    return false;
    }
  );

  screen.Loop(renderer);
}
