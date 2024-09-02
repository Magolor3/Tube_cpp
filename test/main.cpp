#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>

const double e_diameter = 5.0; // Minimum necessary diameter

class Tube {
public:
    std::string name;
    double length;
    double outer_diameter;
    double inner_diameter;
    double wall_thickness;
    double curvature;

    Tube(std::string n, double l, double od, double id, double wt, double c)
        : name(n), length(l), outer_diameter(od), inner_diameter(id), wall_thickness(wt), curvature(c) {}

    bool operator==(const Tube &other) const {
        return std::tie(name, length, outer_diameter, inner_diameter, wall_thickness, curvature) ==
               std::tie(other.name, other.length, other.outer_diameter, other.inner_diameter, other.wall_thickness, other.curvature);
    }

    bool operator<(const Tube &other) const {
        return std::tie(name, length, outer_diameter, inner_diameter, wall_thickness, curvature) <
               std::tie(other.name, other.length, other.outer_diameter, other.inner_diameter, other.wall_thickness, other.curvature);
    }

    std::string to_string() const {
        std::ostringstream oss;
        oss << name << " (Length: " << length << ", Outer Diameter: " << outer_diameter
            << ", Inner Diameter: " << inner_diameter << ", Wall Thickness: " << wall_thickness << ")";
        return oss.str();
    }
};

//read parameters from files


bool is_valid_combination(const Tube &inner, const Tube &outer) {
    return (inner.length > outer.length &&
            inner.outer_diameter < outer.inner_diameter &&
            inner.outer_diameter >= outer.inner_diameter - e_diameter &&
            inner.wall_thickness <= outer.wall_thickness);
}

std::set<std::vector<Tube>> find_valid_combinations(const std::vector<Tube> &tubes, int n) {
    std::set<std::vector<Tube>> valid_combinations;
    if (n == 1) {
        for (const auto &tube : tubes) {
            valid_combinations.insert({tube});
        }
    } else {
        std::vector<bool> v(tubes.size());
        std::fill(v.end() - n, v.end(), true);
        do {
            std::vector<Tube> combination;
            for (size_t i = 0; i < tubes.size(); ++i) {
                if (v[i]) combination.push_back(tubes[i]);
            }
            if (combination.size() == n) {
                bool valid = true;
                for (size_t i = 0; i < combination.size() - 1 && valid; ++i) {
                    if (!is_valid_combination(combination[i], combination[i + 1])) {
                        valid = false;
                    }
                }
                if (valid) valid_combinations.insert(combination);
            }
        } while (std::next_permutation(v.begin(), v.end()));
    }
    return valid_combinations;
}

void print_combinations_table(const std::vector<Tube> &tubes, int n) {
    std::cout << "Tube Combination Information for " << n << " tube(s):\n";
    if (tubes.size() == 1) {
        std::cout << tubes[0].to_string() << "\t|\t";
    } else {
        std::set<std::vector<Tube>> combinations = find_valid_combinations(tubes, n);
        if (combinations.empty()) {
            std::cout << "No valid combinations found.\n";
        } else {
            for (const auto &combo : combinations) {
                const Tube &inner_tube = combo[0];
                std::string outer_tubes_info;
                for (size_t i = 1; i < combo.size(); ++i) {
                    outer_tubes_info += combo[i].to_string();
                    if (i < combo.size() - 1) outer_tubes_info += " , ";
                }
                std::cout << inner_tube.to_string() << "\t|\t" << outer_tubes_info << "\n";
            }
        }
    }
}

int main() {
    std::vector<Tube> tubes = read_parameters();
    for (int n = 1; n <= tubes.size(); ++n) {
        print_combinations_table(tubes, n);
    }

    int n;
    std::cout << "Enter the number of Tubes :)    n: ";
    std::cin >> n;
    print_combinations_table(tubes, n);

    return 0;
}