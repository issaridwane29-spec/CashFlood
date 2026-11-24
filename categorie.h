#ifndef CATEGORIE_H
#define CATEGORIE_H

#include <QString>

class Categorie
{
public:
    Categorie();
    Categorie(int id, const QString &nom, const QString &couleur);
    int id() const;
    void setId(int id);

    QString nom() const;
    void setNom(const QString &nom);

    QString couleur() const;
    void setCouleur(const QString &couleur);

private:
    int m_id;
    QString m_nom;
    QString m_couleur;

};

#endif // CATEGORIE_H
