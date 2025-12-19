// Compile : g++ -O2 zeckendorf.cpp -o zeckendorf `pkg-config --cflags --libs flint`
#include <iostream>
#include <flint/fmpz.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <n-as-decimal-string>" << endl;
        return 1;
    }

    fmpz_t n;
    fmpz_init(n);
    if (fmpz_set_str(n, argv[1], 10) != 0) {
        cerr << "Error: invalid value for n" << endl;
        return 2;
    }

    const int MAX_FIB = 10000;
    fmpz_t fib[MAX_FIB];
    int count = 0;

    fmpz_init(fib[0]); fmpz_set_ui(fib[0], 1); count++;
    fmpz_init(fib[1]); fmpz_set_ui(fib[1], 2); count++;

    while (true) {
        if (count >= MAX_FIB) {
          cerr << "MAX_FIB too low" << endl;
          return 3;
        }

        fmpz_init(fib[count]);
        fmpz_add(fib[count], fib[count-1], fib[count-2]);

        if (fmpz_cmp(fib[count], n) > 0) {
            fmpz_clear(fib[count]);
            break;
        }
        count++;
    }

    bool first = true;
    fmpz_t rem;
    fmpz_init_set(rem, n);

    cout << "n = ";
    for (int i = count-1; i >= 0; i--) {
        if (fmpz_cmp(fib[i], rem) <= 0) {
            if (!first) cout << " + ";
            fmpz_print(fib[i]);
            first = false;

            fmpz_sub(rem, rem, fib[i]);

            // Zeckendorf : on saute F_{k-1} pour éviter 2 consécutifs
            i--;

            if (fmpz_is_zero(rem)) break;
        }
    }
    cout << endl;

    // 3. Nettoyage
    for (int i = 0; i < count; i++)
        fmpz_clear(fib[i]);
    fmpz_clear(rem);
    fmpz_clear(n);

    return 0;
}
