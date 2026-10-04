#ifndef PILE_DLL_H
#define PILE_DLL_H

/*
    ==========================================================
    EN-TETE PUBLIC DE LA BIBLIOTHEQUE pile.dll
    ==========================================================
    - Quand on COMPILE la DLL   : on definit BUILD_DLL  -> dllexport
    - Quand on UTILISE la DLL   : BUILD_DLL absent      -> dllimport
*/
#if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef BUILD_DLL
        #define PILE_API __declspec(dllexport)
    #else
        #define PILE_API __declspec(dllimport)
    #endif
#else
    #define PILE_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Type de pile a creer */
#define PILE_TABLEAU 0
#define PILE_LISTE   1

/* "Poignee" opaque : l'utilisateur ne voit pas le contenu de la pile */
typedef void* HPile;

/* ---------- Fonctions de manipulation de la pile (de char) ---------- */
PILE_API HPile pile_creer(int typePile);              /* NULL si type invalide */
PILE_API void  pile_detruire(HPile p);
PILE_API void  pile_empiler(HPile p, char c);
PILE_API int   pile_depiler(HPile p, char* resultat); /* 1 = ok, 0 = pile vide */
PILE_API int   pile_sommet(HPile p, char* resultat);  /* 1 = ok, 0 = pile vide */
PILE_API int   pile_estVide(HPile p);                 /* 1 = vide, 0 = non vide */
PILE_API int   pile_taille(HPile p);

/* ---------- Validation d'une expression ----------
   typePile : PILE_TABLEAU ou PILE_LISTE
   message  : tampon (fourni par l'appelant) qui recevra l'explication
   Retour   : 1 = expression valide, 0 = invalide                      */
PILE_API int verifier_expression(const char* expr, int typePile,
                                 char* message, int tailleMessage);

#ifdef __cplusplus
}
#endif

#endif
