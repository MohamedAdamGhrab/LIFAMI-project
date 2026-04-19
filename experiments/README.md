# Experiments — Historique du développement

Ce dossier retrace les étapes du projet, du premier dépôt avec triangles jusqu'à la version finale.

▶️ **[Vidéo de démonstration complète sur YouTube](https://youtu.be/VnwFoeqomjY)**

---

## Dépôt 1 — Triangles (~900 lignes)

### Ce que contenait ce code
Deux espèces seulement (lapins + loups), pas de vaches, pas de saisons, pas de courbes, pas de météorites.

**Représentation** : triangles pleins orientés selon la direction. La direction était recalculée à chaque frame dans la fonction de dessin — en pause, les triangles tournaient sur eux-mêmes. Bug connu, documenté dans le commentaire du code, corrigé dans la version finale.

**Affichage** : utilisait `<string>` et `to_string()` pour construire les textes du panneau. Dans la version finale, le code a été rendu **conforme aux fonctions vues en TD/TP** avec `print(x, y, int)` directement.

**Vitesse** : compteur fixe à 80 dans le main. Dans la version finale, ce seuil devient `11 - m.vitesse`, réglable en temps réel.

**Ce qui fonctionnait déjà :**
- Niveaux 1 à 4 (lapins via échappements, loups via repas)
- Meutes de loups, `verifier_meutes()` à chaque frame
- Anticipation de trajectoire (loups niv.2, `px_avant/py_avant`)
- Fuite intelligente (lapins niv.3-4)
- Compactage des morts, reproduction avec contrôle de densité

### Screenshots dépôt 1

**Début de simulation** (frame 2) — 56 lapins (triangles bleus), 8 loups (triangles rouges), tous niveau 1 :

![Début simulation dépôt 1](screenshots/debut_simulation.png)

**Quasi-extinction** (frame 412) — 11 lapins, 1 loup. Les loups avaient atteint les niveaux 2 et 3 avant de décimer la population :

![Quasi extinction dépôt 1](screenshots/quasi_extinction_lapins.png)

**Extinction des loups** (frame 682) — Loups:0, 10 lapins survivants. L'herbe reconquiert toute la carte. Illustration de l'effondrement Lotka-Volterra :

![Extinction loups dépôt 1](screenshots/extinction_loups.png)

---

## Version finale — Cercles, 3 espèces, événements (~2100 lignes)

### Changements visuels principaux

Triangles remplacés par **disques + ligne blanche de direction** — plus lisible, plus rapide à dessiner, et définitivement exempt du bug de rotation :

```cpp
// Dépôt 1 : triangle rasterisé manuellement
remplir_triangle(ax, ay, bx, by, cx, cy);

// Version finale : disque + ligne
circleFill(px, py, taille);
color(255, 255, 255);
line(px, py, px + dx*taille, py + dy*taille);
```

### Fonctionnalités ajoutées
- Vaches (3e espèce)
- Fermiers : zones de protection cliquables (cercles bleus)
- Météorites : cratère + onde de choc
- Incendies : propagation avec double buffer
- 4 saisons avec boutons cliquables
- Modèle SIR avec comptage par espèce
- 5 courbes Lotka-Volterra temps réel
- Vitesse dynamique (touches + et 6)

### Screenshots version finale

**Début simulation** — 25 lapins, 15 loups, 7 vaches, panneau avec courbes démarrées :

![Début simulation finale](screenshots/debut_simulation.png)

**Météorites + Fermiers + Incendie** (cycle 35) — cratères marron foncé, cercles bleus de protection, feu jaune bas-gauche, cercles jaunes = animaux infectés :

![Météorites fermiers incendie](screenshots/meteorites_fermiers.png)

**Incendie massif + Cendres** (cycle 66) — le feu orange laisse des cendres grises (CENDRE), les animaux restants fuient. Les fermiers maintiennent leurs zones protégées :

![Incendie complet](screenshots/incendie_complet.png)

**Quasi-extinction + Courbes LV** (cycle 602, saison Été) — 4 lapins, loups éteints. Les courbes montrent la divergence entre simulation (escalier bleu) et théorie (courbe cyan) provoquée par les catastrophes :

![Quasi extinction courbes](screenshots/quasi_extinction_courbes.png)

---

## Résumé des évolutions

| | Dépôt 1 | Version finale |
|--|---------|----------------|
| Lignes | ~900 | ~2100 |
| Espèces | 2 | 3 |
| Représentation | Triangles | Cercles + ligne blanche |
| Affichage | `<string>`, `to_string()` | `print()` Grapic (conforme TD) |
| Bug rotation | Présent | Corrigé (`dir_x/dir_y`) |
| Bug pause | Présent | Corrigé (`espace_avant`) |
| Courbes LV | Non | Oui (5 courbes) |
| Saisons | Non | Oui |
| Épidémie SIR | Non | Oui |
| Météorites/Incendies | Non | Oui |
| Vitesse | Fixe (80) | Dynamique (`11-vitesse`) |
