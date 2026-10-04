#ifndef PILE_LISTE_CHAINEE_HPP
#define PILE_LISTE_CHAINEE_HPP

#include <iostream>
#include <stdexcept>

/*
    ==========================================================
    PILE GENERIQUE - IMPLEMENTATION PAR LISTE CHAINEE
    ==========================================================
    Principe :
    - Chaque élément est stocké dans un "maillon" (nœud) alloué
      dynamiquement sur le tas (new / delete)
    - Le pointeur "sommet" pointe toujours vers le dernier
      élément empilé
    - Complexité : O(1) pour empiler/dépiler, pas de redimensionnement
      nécessaire (pas de limite de capacité fixe)
*/

template <typename T>
class PileListe {
private:
    // Structure interne représentant un maillon de la liste
    struct Maillon {
        T valeur;
        Maillon* suivant;
        Maillon(const T& v, Maillon* s) {
            valeur = v;
            suivant = s;
        }
    };

    Maillon* sommet;   // pointeur vers le sommet de la pile
    int nbElements;    // compteur d'éléments

public:
    // Constructeur
    PileListe() {
        sommet = NULL;
        nbElements = 0;
    }

    // Destructeur : libère tous les maillons restants
    ~PileListe() {
        while (!estVide()) {
            depiler();
        }
    }

    // Interdiction de copie simple
    PileListe(const PileListe&) = delete;
    PileListe& operator=(const PileListe&) = delete;

    // Teste si la pile est vide
    bool estVide() const {
        return sommet == NULL;
    }

    // Empile un élément : crée un nouveau maillon en tête de liste
    void empiler(const T& valeur) {
        Maillon* nouveau = new Maillon(valeur, sommet);
        sommet = nouveau;
        nbElements++;
    }

    // Dépile un élément : retire le maillon de tête
    T depiler() {
        if (estVide()) {
            throw std::runtime_error("Erreur : depiler() sur une pile vide");
        }
        Maillon* ancienSommet = sommet;
        T valeur = ancienSommet->valeur;
        sommet = sommet->suivant;
        delete ancienSommet;   // libération de la mémoire du tas
        nbElements--;
        return valeur;
    }

    // Consulte le sommet sans le retirer
    T sommetPile() const {
        if (estVide()) {
            throw std::runtime_error("Erreur : sommetPile() sur une pile vide");
        }
        return sommet->valeur;
    }

    // Retourne le nombre d'éléments dans la pile
    int taille() const {
        return nbElements;
    }

    // Affiche la pile du sommet vers la base
    void afficher() const {
        std::cout << "Pile (liste chainee) [sommet -> base] : ";
        if (estVide()) {
            std::cout << "(vide)";
        }
        Maillon* courant = sommet;
        while (courant != NULL) {
            std::cout << courant->valeur << " ";
            courant = courant->suivant;
        }
        std::cout << std::endl;
    }
};

#endif
