#include <iostream>
#include "pile_dll.h"     // seul en-tete necessaire pour utiliser la DLL

using namespace std;

void tester(const char* expr) {
    char message[200];
    cout << "Expression : " << expr << endl;

    int r1 = verifier_expression(expr, PILE_TABLEAU, message, 200);
    cout << "  [Tableau] " << (r1 ? "VALIDE  : " : "INVALIDE: ") << message << endl;

    int r2 = verifier_expression(expr, PILE_LISTE, message, 200);
    cout << "  [Liste]   " << (r2 ? "VALIDE  : " : "INVALIDE: ") << message << endl;
    cout << endl;
}

int main()
{
    // ---------- Partie 1 : manipulation directe de la pile via la DLL ----------
    cout << "===== Manipulation directe de la pile (liste chainee) =====" << endl;

    HPile p = pile_creer(PILE_LISTE);
    pile_empiler(p, 'A');
    pile_empiler(p, 'B');
    pile_empiler(p, 'C');
    cout << "Taille : " << pile_taille(p) << endl;

    char c;
    if (pile_sommet(p, &c)) cout << "Sommet : " << c << endl;

    while (!pile_estVide(p)) {
        pile_depiler(p, &c);
        cout << "Depile : " << c << endl;
    }

    if (!pile_depiler(p, &c)) cout << "Depiler sur pile vide -> refuse (retour 0)" << endl;

    pile_detruire(p);
    cout << endl;

    // ---------- Partie 2 : validation d'expressions ----------
    cout << "===== Validation d'expressions =====" << endl;
    tester("(a+b)*[c-d]");
    tester("{[(a+b)*c]-d}");
    tester("(a+b]");
    tester("(a+b))");
    tester("((a+b)");
    tester(")a+b(");
    tester("a+b*c");

    return 0;
}
