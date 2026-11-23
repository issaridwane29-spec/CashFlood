#ifndef OPERATION_H
#define OPERATION_H

#include <QString>
#include <QDate>


class Operation
{
public:
    Operation();
    Operation(int id, const QString &type, double montant, const QDate &date, const QString &description, int idCompte, int idCategorie);
    int id() const;
    void setId(int id);

    QString type() const;
    void setType(const QString &type);

    double montant() const;
    void setMontant(double montant);

    QDate date() const;
    void setDate(const QDate &date);

    QString description() const;
    void setDescription(const QString &description);

    int idCompte() const;
    void setIdCompte(int idCompte);

    int idCategorie() const;
    void setIdCategorie(int idCategorie);

private:
    int m_id;
    QString m_nom;
    QString m_type;
    double m_montant;
    QDate m_date;
    QString m_description;
    int m_idCompte;
    int m_idCategorie;
};

#endif // OPERATION_H
