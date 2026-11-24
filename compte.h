#ifndef COMPTE_H
#define COMPTE_H

#include <QString>


class Compte
{
public:
    Compte();
    Compte(int id, const QString &nom, const QString &type, double soldeInitial, int ordre);
    int id() const;
    void setId(int id);

    QString nom() const;
    void setNom(const QString &nom);

    QString type() const;
    void setType(const QString &type);

    double soldeInitial() const;
    void setSoldeInitial(double solde);

    int ordre() const;
    void setOrdre(int ordre);

private:
    int m_id;
    QString m_nom;
    QString m_type;
    double m_soldeInitial;
    int m_ordre;
};

#endif // COMPTE_H
