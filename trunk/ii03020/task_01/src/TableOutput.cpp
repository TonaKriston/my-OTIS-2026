#include "TableOutput.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

vector<SimRow> simulate(Model& m, int steps) {
    vector<SimRow> rows;
    rows.reserve(steps + 1);
    rows.push_back(SimRow{0, m.GetY(), 0.0, false});
    for (int k = 1; k <= steps; ++k) {
        double y = m.NextStep();
        rows.push_back(SimRow{k, y, m.GetLastU(), true});
    }
    return rows;
}

void write_table(const vector<SimRow>& rows, ostream& out, char sep) {
    out << "step" << sep << "y" << sep << "u_used\n";
    out << "==================================\n";
    out << fixed << setprecision(6);
    for (const auto& r : rows) {
        out << r.step << sep << r.y << sep;
        if (r.u_valid) out << r.u;
        else           out << "-";
        out << "\n";
    }
    out << "==================================\n";
    out << defaultfloat;
}

bool save_table_to_file(const vector<SimRow>& rows, const string& filename) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Ошибка: не удалось открыть файл " << filename << "\n";
        return false;
    }
    write_table(rows, fout, ',');
    fout.close();
    cout << "Таблица сохранена в файл: " << filename << "\n";
    return true;
}

bool run_and_report(Model& m, int steps,
                    const string& title,
                    const string& filename) {
    auto rows = simulate(m, steps);
    cout << "\n=== " << title << " ===\n";
    write_table(rows, cout, '\t');
    return save_table_to_file(rows, filename);
}