#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

template<typename Consumer>
void getPermutations(int n, int k, Consumer consume) {
    if (n < k) {
        return;
    }

    std::vector<bool> bitmask(k, true); // k leading 1's
    bitmask.resize(n, false); // n-k trailing 0's

    do {
        std::vector<int> permutation;
        permutation.reserve(n);

        for (int i = 0; i < n; ++i) {
            if (bitmask[i]) {
                permutation.push_back(i);
            };
        }

        do {
            consume(permutation);
        } while (std::next_permutation(permutation.begin(), permutation.end()));

    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}

int main() {
    getPermutations(10, 5, [](const auto &permutation) {
        std::copy(permutation.begin(), permutation.end(), std::ostream_iterator<int>(std::cout));
        std::cout << std::endl;
    });
}
