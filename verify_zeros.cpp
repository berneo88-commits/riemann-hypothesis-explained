#include <flint/acb.h>
#include <flint/acb_dirichlet.h>
#include <flint/arb.h>

#include <fstream>
#include <iostream>
#include <string>

constexpr slong precision = 120;

int hardy_sign(const arb_t t)
{
    acb_t input, value;
    acb_init(input);
    acb_init(value);
    acb_set_arb(input, t);
    // Null group and character select the Riemann zeta Hardy Z-function.
    acb_dirichlet_hardy_z(value, input, nullptr, nullptr, 1, precision);
    const auto real = acb_realref(value);
    const int sign = arb_is_positive(real) - arb_is_negative(real);
    acb_clear(value);
    acb_clear(input);
    return sign;
}

int main(int argc, char **argv)
{
    const char *path = argc == 2 ? argv[1] : "zeta_zeros_2000.txt";
    std::ifstream input(path);
    if (!input || argc > 2) {
        std::cerr << "Usage: verify_zeros [candidate-heights.txt]\n";
        return 2;
    }

    arb_t height, center, epsilon, lower, upper, previous_upper, total;
    arb_init(height);
    arb_init(center);
    arb_init(epsilon);
    arb_init(lower);
    arb_init(upper);
    arb_init(previous_upper);
    arb_init(total);

    arb_set_ui(height, 40245);
    arb_mul_2exp_si(height, height, -4); // 2515.3125, exactly
    arb_one(epsilon);
    arb_mul_2exp_si(epsilon, epsilon, -10); // 1/1024, exactly

    long brackets = 0;
    bool valid = true;
    std::string line;
    while (std::getline(input, line)) {
        if (arb_set_str(center, line.c_str(), precision) != 0) {
            valid = false;
            break;
        }
        arb_sub(lower, center, epsilon, precision);
        arb_add(upper, center, epsilon, precision);
        if (!arb_lt(previous_upper, lower) || !arb_lt(upper, height) ||
            hardy_sign(lower) * hardy_sign(upper) != -1) {
            valid = false;
            break;
        }
        arb_set(previous_upper, upper);
        ++brackets;
    }

    acb_dirichlet_zeta_nzeros(total, height, precision);
    const bool verified = valid && input.eof() && !input.bad() && brackets == 2000 &&
                          arb_equal_si(total, brackets);
    std::cout << "Disjoint sign-changing Hardy Z brackets: " << brackets << '\n';
    std::cout << "Total zeros through height 2515.3125: ";
    arb_printn(total, 20, ARB_STR_NO_RADIUS);
    std::cout << '\n';
    std::cout << (verified ? "VERIFIED: all 2000 zeros are simple and on the critical line."
                           : "VERIFICATION FAILED.") << '\n';

    arb_clear(total);
    arb_clear(previous_upper);
    arb_clear(upper);
    arb_clear(lower);
    arb_clear(epsilon);
    arb_clear(center);
    arb_clear(height);
    return verified ? 0 : 1;
}
