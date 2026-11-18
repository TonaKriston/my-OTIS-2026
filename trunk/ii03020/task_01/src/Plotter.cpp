#include "Plotter.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

namespace {

// Записывает готовый текст в файл, возвращает false при ошибке.
bool write_file(const std::string& path, const std::string& content) {
    std::ofstream f(path);
    if (!f) {
        std::cerr << "Ошибка: не удалось открыть файл " << path << "\n";
        return false;
    }
    f << content;
    return true;
}

} // namespace

bool Plotter::is_gnuplot_available() {
#ifdef _WIN32
    int rc = std::system("gnuplot --version >nul 2>&1");
#else
    int rc = std::system("gnuplot --version >/dev/null 2>&1");
#endif
    return rc == 0;
}

bool Plotter::run_gnuplot(const std::string& plot_file) {
    std::string cmd = "gnuplot \"" + plot_file + "\"";
    if (int rc = std::system(cmd.c_str()); rc != 0) {
        std::cerr << "Не удалось выполнить gnuplot. Проверьте, что он "
                     "установлен и прописан в PATH.\n"
                  << "Скрипт сохранён: " << plot_file
                  << " — его можно открыть вручную.\n";
        return false;
    }
    return true;
}

bool Plotter::plot_csv(const std::string& csv_file,
                       const std::string& plot_file,
                       const std::string& png_file,
                       const std::string& title,
                       bool invoke_gnuplot) {
    // every ::1 — пропустить строку заголовка CSV (step,y,u_used)
    std::string s;
    s += "set terminal pngcairo size 1000,700 enhanced font 'Verdana,10'\n";
    s += "set output '" + png_file + "'\n";
    s += "set encoding utf8\n";
    s += "set datafile separator ','\n";
    s += "set datafile missing '-'\n";
    s += "set grid\n";
    s += "set multiplot layout 2,1 title '" + title + "'\n";

    // --- панель y(step) ---
    s += "set xlabel 'step'\n";
    s += "set ylabel 'y'\n";
    s += "plot '" + csv_file + "' every ::1 using 1:2 "
         "with linespoints lw 2 pt 7 lc rgb '#1f77b4' title 'y(step)'\n";

    // --- панель u(step) ---
    s += "set ylabel 'u'\n";
    s += "plot '" + csv_file + "' every ::1 using 1:3 "
         "with linespoints lw 2 pt 5 lc rgb '#d62728' title 'u(step)'\n";

    s += "unset multiplot\n";

    if (!write_file(plot_file, s)) return false;

    if (invoke_gnuplot) return run_gnuplot(plot_file);
    return true;
}

bool Plotter::plot_comparison(const std::vector<std::string>& csv_files,
                              const std::vector<std::string>& legends,
                              const std::string& plot_file,
                              const std::string& png_file,
                              const std::string& title,
                              bool invoke_gnuplot) {
    if (csv_files.size() != legends.size()) {
        std::cerr << "Ошибка: количество CSV и подписей не совпадает\n";
        return false;
    }

    std::string s;
    s += "set terminal pngcairo size 1200,700 enhanced font 'Verdana,11'\n";
    s += "set output '" + png_file + "'\n";
    s += "set encoding utf8\n";
    s += "set datafile separator ','\n";
    s += "set title '" + title + "'\n";
    s += "set xlabel 'step'\n";
    s += "set ylabel 'y'\n";
    s += "set grid\n";
    s += "set key top left\n";

    s += "plot ";
    for (size_t i = 0; i < csv_files.size(); ++i) {
        if (i > 0) s += ", \\\n     ";
        s += "'" + csv_files[i] + "' every ::1 using 1:2 "
             "with linespoints lw 2 title '" + legends[i] + "'";
    }
    s += "\n";

    if (!write_file(plot_file, s)) return false;

    if (invoke_gnuplot) return run_gnuplot(plot_file);
    return true;
}