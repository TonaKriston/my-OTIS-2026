#pragma once
#include <string>
#include <vector>

namespace Plotter {

// Строит PNG с двумя панелями (y(step) и u(step)) по одному CSV.
// Всегда генерирует .plt-скрипт. Если invoke_gnuplot == true — пытается
// сразу выполнить его через gnuplot.
bool plot_csv(const std::string& csv_file,
              const std::string& plot_file,
              const std::string& png_file,
              const std::string& title,
              bool invoke_gnuplot = true);

// Строит один PNG, накладывая y(step) из нескольких CSV на общий график.
bool plot_comparison(const std::vector<std::string>& csv_files,
                     const std::vector<std::string>& legends,
                     const std::string& plot_file,
                     const std::string& png_file,
                     const std::string& title,
                     bool invoke_gnuplot = true);

// Запускает gnuplot на указанном .plt-файле. Возвращает false, если
// gnuplot не установлен или завершился с ошибкой.
bool run_gnuplot(const std::string& plot_file);

// Проверяет, доступен ли gnuplot в PATH.
bool is_gnuplot_available();

} // namespace Plotter