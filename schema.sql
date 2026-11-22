CREATE TABLE IF NOT EXISTS "budget" (
	"idbud"	INTEGER,
	"nombud"	TEXT NOT NULL,
	"plafond"	REAL NOT NULL CHECK("plafond" > 0),
	"periode"	TEXT NOT NULL DEFAULT 'mensuel' CHECK("periode" IN ('hebdomadaire', 'mensuel', 'annuel')),
	PRIMARY KEY("idbud" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "budget_categorie" (
	"idbud"	INTEGER NOT NULL,
	"idcat"	INTEGER NOT NULL,
	PRIMARY KEY("idbud","idcat"),
	FOREIGN KEY("idbud") REFERENCES "budget"("idbud") ON DELETE CASCADE,
	FOREIGN KEY("idcat") REFERENCES "categorie"("idcat") ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "categorie" (
	"idcat"	INTEGER,
	"nomcat"	TEXT NOT NULL UNIQUE,
	"couleur"	TEXT NOT NULL DEFAULT '#888888',
	PRIMARY KEY("idcat" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "compte" (
	"idcmpt"	INTEGER,
	"nomcmpt"	TEXT NOT NULL UNIQUE,
	"typecmpt"	TEXT NOT NULL,
	"soldeinitial"	REAL NOT NULL DEFAULT 0,
	"ordreaffichage"	INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY("idcmpt" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "operation" (
	"idope"	INTEGER,
	"idcmpt"	INTEGER NOT NULL,
	"idcat"	INTEGER,
	"typeope"	TEXT NOT NULL CHECK("typeope" IN ('revenu', 'depense')),
	"montant"	REAL NOT NULL CHECK("montant" > 0),
	"dateope"	TEXT NOT NULL,
	"description"	TEXT NOT NULL DEFAULT '',
	PRIMARY KEY("idope" AUTOINCREMENT),
	FOREIGN KEY("idcat") REFERENCES "categorie"("idcat") ON DELETE SET NULL,
	FOREIGN KEY("idcmpt") REFERENCES "compte"("idcmpt") ON DELETE CASCADE
);
