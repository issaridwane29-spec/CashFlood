#ifndef BUDGET_H
#define BUDGET_H

#include <QString>
#include <QList>

class Budget
{
public:
    Budget();
    Budget(int id, const QString &nom, double plafond,
           const QString &periode, const QList<int> &categories);

    int id() const;
    void setId(int id);

    QString nom() const;
    void setNom(const QString &nom);

    double plafond() const;
    void setPlafond(double plafond);

    QString periode() const;
    void setPeriode(const QString &periode);

    QList<int> categories() const;
    void setCategories(const QList<int> &categories);

private:
    int m_id;
    QString m_nom;
    double m_plafond;
    QString m_periode;
    QList<int> m_categories;
};

#endif // BUDGET_H
