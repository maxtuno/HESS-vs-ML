/*
Copyright (c) 2012-2021 Oscar Riveros [https://twitter.com/maxtuno].

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <fstream>
#include <limits>

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    for (auto &e: vec) {
        os << e << " ";
    }
    return os;
}

template<typename T>
T dot(const std::vector<T> &a, const std::vector<T> &b) {
    T local = 0;
    for (auto i{0}; i < a.size(); i++) {
        local += a[i] * b[i];
    }
    return local;
}

template<typename T>
T oracle(const std::vector<T> &w, const T &c, const std::vector<std::vector<T>> &data, const std::vector<T> &labels, const std::size_t &n, const double &global) {
    T local = n;
    for (auto i{0}; i < n; i++) {
        local -= 1 - labels[i] * (dot(w, data[i]) + c) <= 0;
        if (local + i <= global) {
            return local;
        }
    }
    return local;
}

template<typename T>
std::pair<T, std::vector<T>> hess(const std::vector<std::vector<T>> &data, const std::vector<T> &labels, const double &min_val, const double &max_val, const std::size_t &n, const double &percent) {
    std::cout.precision(std::numeric_limits<T>::max_digits10 + 1);
    T global = std::numeric_limits<T>::max();

    std::vector<T> w_opt;
    T c_opt;

    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_real_distribution<T> distribution(n * min_val, n * max_val);

    std::vector<T> a(n);
    std::vector<T> b(n);

    std::generate(a.begin(), a.end(), [&]() { return distribution(engine); });
    std::generate(b.begin(), b.end(), [&]() { return distribution(engine); });
    closure:
    for (auto i{0}; i < n; i++) {
        std::swap(a[i], b[n - 1 - i]);
        auto c = distribution(engine);
        auto w = {a[i], b[n - 1 - i]};
        auto local = oracle<T>(w, c, data, labels, n, global);
        if (local < global) {
            global = local;
            c_opt = c;
            w_opt.assign(w.begin(), w.end());
            std::cout << global << std::endl;
            if (global <= n * percent) {
                break;
            }
            goto closure;
        } else if (local > global) {
            std::swap(a[i], b[n - 1 - i]);
        }
    }
    return std::make_pair(c_opt, w_opt);
}

int main(int argc, char *argv[]) {
    using T = double;

    std::vector<std::vector<T>> data;
    std::vector<T> labels;

    T x, y, label, min_val = std::numeric_limits<T>::max(), max_val = std::numeric_limits<T>::min(), percent = std::atof(argv[3]);
    std::ifstream data_file(argv[1]);
    std::ifstream labels_file(argv[2]);
    for (;;) {
        data_file >> x >> y;
        if (x < min_val) {
            min_val = x;
        }
        if (x > max_val) {
            max_val = x;
        }
        if (y < min_val) {
            min_val = y;
        }
        if (y > max_val) {
            max_val = y;
        }
        labels_file >> label;

        if (labels_file.eof() || data_file.eof()) { break; }

        data.emplace_back(std::vector{x, y});
        labels.emplace_back(label == 0 ? -1 : 1);
    }
    labels_file.close();
    data_file.close();

    std::cout << data.size() << std::endl;

    auto opt = hess<T>(data, labels, min_val, max_val, data.size(), percent);

    auto c_opt = opt.first;
    auto w_opt = opt.second;

    std::cout << "python3 plot_hess.py " << c_opt << " [" << w_opt[0] << "," << w_opt[1] << "]" << std::endl;

    return EXIT_SUCCESS;
}