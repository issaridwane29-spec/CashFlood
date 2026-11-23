#include "budget.h"

Budget::Budget() : m_id(0), m_plafond(0) {}

Budget::Budget(int id, const QString &nom, double plafond,
               const QString &periode, const QList<int> &categories)
    : m_id(id), m_nom(nom), m_plafond(plafond),
      m_periode(periode), m_categories(categories) {}

int Budget::id() const { return m_id; }
void Budget::setId(int id) { m_id = id; }

QString Budget::nom() const { return m_nom; }
void Budget::setNom(const QString &nom) { m_nom = nom; }

double Budget::plafond() const { return m_plafond; }
void Budget::setPlafond(double plafond) { m_plafond = plafond; }

QString Budget::periode() const { return m_periode; }
void Budget::setPeriode(const QString &periode) { m_periode = periode; }

QList<int> Budget::categories() const { return m_categories; }
void Budget::setCategories(const QList<int> &categories) { m_categories = categories; }
