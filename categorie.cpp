#include "categorie.h"

Categorie::Categorie() : m_id(0) {}

Categorie::Categorie(int id, const QString &nom, const QString &couleur)
    : m_id(id), m_nom(nom), m_couleur(couleur) {}

int Categorie::id() const { return m_id; }
void Categorie::setId(int id) { m_id = id; }

QString Categorie::nom() const {
    return m_nom;
}
void Categorie::setNom(const QString &nom){ m_nom = nom; }

QString Categorie::couleur() const { return m_couleur; }
void Categorie::setCouleur(const QString &couleur) { m_couleur = couleur; }
