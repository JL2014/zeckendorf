// Compile : g++ -O2 zeckendorf.cpp -o zeckendorf `pkg-config --cflags --libs flint`
#include <iostream>
#include <vector>
#include <flint/fmpz.h>

using namespace std;

struct Big {
    fmpz_t v;
    Big() { fmpz_init(v); }
    Big(const Big &b) { fmpz_init(v); fmpz_set(v, b.v); }
    ~Big() { fmpz_clear(v); }
};

void build_fib_sequence(vector<Big> &fib, const fmpz_t n) {
    Big a, b, c;

    fmpz_set_ui(a.v, 1);  // F1
    fmpz_set_ui(b.v, 2);  // F2

    fib.push_back(a);
    fib.push_back(b);

    while (true) {
        fmpz_add(c.v, a.v, b.v);
        if (fmpz_cmp(c.v, n) > 0)
            break;

        fib.push_back(c);
        Big tmp = b;
        b = c;
        a = tmp;
    }
}

void zeckendorf(const fmpz_t n) {
    vector<Big> fib;
    build_fib_sequence(fib, n);

    fmpz_t r;
    fmpz_init_set(r, n);
    vector<Big> out;
    Big c;

    for (int i = fib.size() - 1; i >= 0; i--) {
        if (fmpz_cmp(fib[i].v, r) <= 0) {
            fmpz_set(c.v, fib[i].v);
            out.push_back(c);

            fmpz_sub(r, r, fib[i].v);

            i--; // règle Zeckendorf : pas de F(k-1)

            if (fmpz_is_zero(r))
                break;
        }
    }

    cerr << "n = ";
    for (size_t i = 0; i < out.size(); i++) {
        fmpz_print(out[i].v);
        if (i + 1 < out.size())
            cout << " + ";
    }
    cout << endl;

    fmpz_clear(r);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: fib <n-as-decimal-string>" << endl;
        return 1;
    }

    fmpz_t n;
    fmpz_init(n);
    if (fmpz_set_str(n, argv[1], 10) != 0) {
        cerr << "Erreur: n invalide\n";
        return 2;
    }

    zeckendorf(n);

    fmpz_clear(n);
    return 0;
}
