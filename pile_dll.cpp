// Si le projet ne definit pas BUILD_DLL, on le fait ici
#ifndef BUILD_DLL
#define BUILD_DLL
#endif

#include "pile_dll.h"

#include <string>
#include <cstring>
#include "pile_tableau.hpp"
#include "pile_liste_chainee.hpp"

/*
    ==========================================================
    PARTIE INTERNE (invisible pour l'utilisateur de la DLL)
    ==========================================================
    Un template ne peut pas etre exporte directement : on utilise donc
    des versions CONCRETES (PileTableau<char> et PileListe<char>), que l'on
    cache derriere une interface commune IPile.
*/

// Interface commune aux deux types de pile
class IPile {
public:
    virtual ~IPile() {}
    virtual void empiler(char c) = 0;
    virtual char depiler() = 0;
    virtual char sommetPile() const = 0;
    virtual bool estVide() const = 0;
    virtual int  taille() const = 0;
};

// Adaptateur : transforme n'importe quelle pile de char en IPile
template <typename P>
class AdaptateurPile : public IPile {
private:
    P pile;
public:
    void empiler(char c)        { pile.empiler(c); }
    char depiler()              { return pile.depiler(); }
    char sommetPile() const     { return pile.sommetPile(); }
    bool estVide() const        { return pile.estVide(); }
    int  taille() const         { return pile.taille(); }
};

// Copie un message dans le tampon de l'appelant, sans depassement
static void copierMessage(const std::string& msg, char* tampon, int taille) {
    if (tampon == NULL || taille <= 0) return;
    int n = (int)msg.size();
    if (n >= taille) n = taille - 1;
    memcpy(tampon, msg.c_str(), n);
    tampon[n] = '\0';
}

// Algorithme de validation (pile de caracteres)
static bool verifier(const std::string& expr, IPile& pile, std::string& msg) {
    for (size_t i = 0; i < expr.size(); i++) {
        char c = expr[i];

        if (c == '(' || c == '[' || c == '{') {
            pile.empiler(c);
        }
        else if (c == ')' || c == ']' || c == '}') {

            if (pile.estVide()) {
                msg = std::string("'") + c + "' en position " + std::to_string(i + 1)
                      + " sans ouverture correspondante";
                return false;
            }

            char ouvrant = pile.depiler();
            bool correspond =
                (ouvrant == '(' && c == ')') ||
                (ouvrant == '[' && c == ']') ||
                (ouvrant == '{' && c == '}');

            if (!correspond) {
                msg = std::string("'") + ouvrant + "' ferme par '" + c
                      + "' en position " + std::to_string(i + 1)
                      + " (mauvaise correspondance)";
                return false;
            }
        }
        // les autres caracteres sont ignores
    }

    if (!pile.estVide()) {
        msg = std::to_string(pile.taille()) + " ouverture(s) jamais fermee(s)";
        return false;
    }

    msg = "Expression valide";
    return true;
}

/*
    ==========================================================
    FONCTIONS EXPORTEES (declarees dans pile_dll.h)
    ==========================================================
    Important : la pile est creee (new) ET detruite (delete) DANS la DLL,
    jamais dans le programme appelant (deux tas memoire differents possibles).
*/
extern "C" {

PILE_API HPile pile_creer(int typePile) {
    IPile* p = NULL;
    if (typePile == PILE_TABLEAU)    p = new AdaptateurPile< PileTableau<char> >();
    else if (typePile == PILE_LISTE) p = new AdaptateurPile< PileListe<char> >();
    return (HPile)p;
}

PILE_API void pile_detruire(HPile p) {
    delete static_cast<IPile*>(p);
}

PILE_API void pile_empiler(HPile p, char c) {
    if (p == NULL) return;
    static_cast<IPile*>(p)->empiler(c);
}

PILE_API int pile_depiler(HPile p, char* resultat) {
    if (p == NULL || resultat == NULL) return 0;
    IPile* pile = static_cast<IPile*>(p);
    if (pile->estVide()) return 0;          // evite de lancer une exception a travers la DLL
    *resultat = pile->depiler();
    return 1;
}

PILE_API int pile_sommet(HPile p, char* resultat) {
    if (p == NULL || resultat == NULL) return 0;
    IPile* pile = static_cast<IPile*>(p);
    if (pile->estVide()) return 0;
    *resultat = pile->sommetPile();
    return 1;
}

PILE_API int pile_estVide(HPile p) {
    if (p == NULL) return 1;
    return static_cast<IPile*>(p)->estVide() ? 1 : 0;
}

PILE_API int pile_taille(HPile p) {
    if (p == NULL) return 0;
    return static_cast<IPile*>(p)->taille();
}

PILE_API int verifier_expression(const char* expr, int typePile,
                                 char* message, int tailleMessage) {
    std::string msg;
    bool ok = false;

    if (expr == NULL) {
        msg = "Expression NULL";
    } else {
        HPile h = pile_creer(typePile);
        if (h == NULL) {
            msg = "Type de pile invalide";
        } else {
            ok = verifier(expr, *static_cast<IPile*>(h), msg);
            pile_detruire(h);
        }
    }

    copierMessage(msg, message, tailleMessage);
    return ok ? 1 : 0;
}

} // extern "C"
