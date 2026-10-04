#ifndef PILE_TABLEAU_HPP
#define PILE_TABLEAU_HPP

#include <iostream>
#include <stdexcept>

/*
    ==========================================================
    PILE GENERIQUE - IMPLEMENTATION PAR TABLEAU
    ==========================================================
    Principe :
    - Mémoire allouée dynamiquement (avec new[]) pour le tableau interne
    - Un indice "sommet" indique la position du dernier élément empilé
    - Complexité : O(1) pour empiler/dépiler (tant qu'il n'y a pas
      de redimensionnement)
*/

template <typename T>
class PileTableau {
private:
    T* tab;          // tableau alloué dynamiquement
    int capacite;    // taille maximale actuelle du tableau
    int sommet;      // indice du sommet (-1 si pile vide)

    // Double la capacité du tableau quand il est plein
    void redimensionner() {
        int nouvelleCapacite = capacite * 2;
        T* nouveauTab = new T[nouvelleCapacite];
        for (int i = 0; i <= sommet; i++) {
            nouveauTab[i] = tab[i];
        }
        delete[] tab;
        tab = nouveauTab;
        capacite = nouvelleCapacite;
    }

public:
    // Constructeur
    PileTableau(int capaciteInitiale = 10) {
        capacite = capaciteInitiale;
        sommet = -1;
        tab = new T[capacite];
    }

    // Destructeur : libère la mémoire allouée dynamiquement
    ~PileTableau() {
        delete[] tab;
    }

    // Interdiction de copie simple (pour éviter les doubles "delete")
    PileTableau(const PileTableau&) = delete;
    PileTableau& operator=(const PileTableau&) = delete;

    // Teste si la pile est vide
    bool estVide() const {
        return sommet == -1;
    }

    // Empile un élément (ajoute au sommet)
    void empiler(const T& valeur) {
        if (sommet + 1 == capacite) {
            redimensionner();
        }
        sommet++;
        tab[sommet] = valeur;
    }

    // Dépile un élément (retire et retourne le sommet)
    T depiler() {
        if (estVide()) {
            throw std::runtime_error("Erreur : depiler() sur une pile vide");
        }
        T valeur = tab[sommet];
        sommet--;
        return valeur;
    }

    // Consulte le sommet sans le retirer
    T sommetPile() const {
        if (estVide()) {
            throw std::runtime_error("Erreur : sommetPile() sur une pile vide");
        }
        return tab[sommet];
    }

    // Retourne le nombre d'éléments dans la pile
    int taille() const {
        return sommet + 1;
    }

    // Affiche la pile du sommet vers la base
    void afficher() const {
        std::cout << "Pile (tableau) [sommet -> base] : ";
        if (estVide()) {
            std::cout << "(vide)";
        }
        for (int i = sommet; i >= 0; i--) {
            std::cout << tab[i] << " ";
        }
        std::cout << std::endl;
    }
};

#endif
