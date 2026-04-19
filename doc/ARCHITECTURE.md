# Architecture du code

## Vue d'ensemble

Fichier source unique `src/simulation.cpp` d'environ **2100 lignes**. Toute la logique passe par un struct `Monde` passé par référence (`Monde &m`) à chaque fonction.

La fenêtre est divisée en deux zones :
- **Grille** : 820 × 700 px → 54 × 46 cases de 15 px (DIMW × DIMH)
- **Panneau** : 280 px à droite (stats, courbes, boutons saisons)

---

## Structures de données

### `struct Animal`
Partagé par lapins, loups et vaches.

| Champ | Rôle |
|-------|------|
| `x, y` | Position en cases sur la grille |
| `px_avant, py_avant` | Position mémorisée toutes les 3 frames — utilisée par le loup niv.2 pour calculer la vitesse du lapin |
| `dir_x, dir_y` | Direction mémorisée dans `deplacer_*()`, lue dans les fonctions de dessin — corrige le bug de rotation en pause |
| `genre` | `Femelle(0)` ou `Male(1)` |
| `energie` | Diminue chaque frame, mort si ≤ 0 |
| `age` | Mort si > `Age_max` |
| `niveau` | 1 à 4, détermine le comportement |
| `nb_echappements` | Lapins : seuils 1/3/6 → niv.2/3/4 |
| `nb_repas` | Loups : seuils 3/6 → niv.2/3 |
| `frames_depuis_repro` | Délai depuis la dernière reproduction |
| `dans_vision_loup` | Vrai au tour précédent — transition true→false = échappement détecté |
| `vivant` | false = mort, compactage en fin de frame |
| `en_meute` | true si loup en meute (niveau 4) |
| `etat` | `SAIN(0)`, `INFECTE(1)`, `RETABLI(2)` |
| `timer_maladie` | Frames depuis infection ou guérison |

### `struct Monde`

| Champ notable | Description |
|---------------|-------------|
| `grille[DIMW][DIMH]` | `vide(0)`, `herbe(1)`, `brule(2)`, `feu(3)`, `cendre(4)` |
| `timer_grille[DIMW][DIMH]` | Compteur pour les transitions brule→vide et cendre→vide |
| `lapins[400]`, `loups[100]`, `vaches[50]` | Tableaux fixes |
| `frame` | Affiché "Cycle" dans le panneau |
| `courbe` | Objet Plot Grapic pour les 5 courbes |
| `lv_N`, `lv_P` | Populations théoriques Lotka-Volterra (float, Euler) |
| `saison` | `PRINTEMPS(0)`, `ETE(1)`, `AUTOMNE(2)`, `HIVER(3)` |
| `nb_infectes_lapins/loups/vaches` | Comptage séparé affiché dans le panneau |
| `vitesse` | 1 à 10 — seuil update = `11 - vitesse` |

---

## Ordre d'appel dans `update()`

```
pousser_herbe()
  ↓
lapins : vieillir → déplacer → manger_herbe
reproduire_lapins()
  ↓
loups : vieillir → déplacer → manger_lapin (+ vaches)
reproduire_loups() → verifier_meutes()
  ↓
vaches : vieillir → déplacer → manger_herbe_vache
reproduire_vaches()
  ↓
update_meteorites() → update_incendie() → propager_maladie()
  ↓
compacter_lapins/loups/vaches()
  ↓
maj_niveaux() → maj_statistiques() → maj_historique()
frame++
```

---

## Algorithmes de direction

| Fonction | Utilisée par | Comportement |
|----------|-------------|--------------|
| `direction_vers_herbe()` | Lapins niv.1, vaches | Case HERBE la plus proche dans un rayon |
| `direction_fuite_aleatoire()` | Lapins niv.2 | Direction aléatoire évitant les loups à distance 1 |
| `direction_fuite_intelligente()` | Lapins niv.3-4 | Maximise la distance au loup le plus proche (8 directions) |
| `direction_vers_lapin()` | Loups niv.1 | Lapin le plus proche dans un rayon |
| `direction_anticipee()` | Loups niv.2 | Prédit position future du lapin via `px_avant` |
| `direction_vers_vache()` | Loups (fallback) | Vache la plus proche |
| `direction_fuite_feu()` | Tous | Fuit à l'opposé du centre de gravité des flammes |
| `direction_fuite_fermier()` | Loups | Fuit le fermier le plus proche |

---

## Dessin des animaux

```cpp
circleFill(px, py, taille);         // disque coloré selon niveau
color(255, 255, 255);
line(px, py, px+dx*taille, py+dy*taille);  // ligne de direction
dessiner_maladie(gx, gy, etat);     // cercle jaune/cyan si malade
```

---

## Incendie — double buffer

```
Copier m.grille → nouvelle_grille
Pour chaque case FEU dans m.grille :
    Tuer animaux sur cette case
    Propager dans nouvelle_grille (pas m.grille)
    Si timer > duree_feu → nouvelle_grille = CENDRE
Recopier nouvelle_grille → m.grille
```

Sans double buffer, le feu brûlerait toute la carte en une frame. Même principe que le Jeu de la Vie (TD9 LIFAMI).

---

## Compactage des morts

```cpp
int w = 0;
for (int i = 0; i < m.nb_lapins; i++)
    if (m.lapins[i].vivant) m.lapins[w++] = m.lapins[i];
m.nb_lapins = w;
```

On marque `vivant=false` pendant la frame, on compacte à la fin pour éviter de sauter des éléments en cours de boucle.# Architecture du code

## Vue d'ensemble

Le projet est un fichier source unique `src/simulation.cpp` d'environ **2100 lignes**, organisé en blocs thématiques séparés par des commentaires. Toute la logique passe par un seul grand struct `Monde` passé par référence (`Monde &m`) à chaque fonction.

La fenêtre est divisée en deux zones :
- **Grille de simulation** : 820 × 700 px (54 × 46 cases de 15 px chacune)
- **Panneau de statistiques** : 280 px à droite

---

## Les structures de données

### `struct Animal`
Utilisé pour les **lapins, les loups et les vaches** — les trois espèces partagent le même struct.

| Champ | Type | Rôle |
|-------|------|------|
| `x, y` | `int` | Position sur la grille en cases (DIMW=54, DIMH=46) |
| `px_avant, py_avant` | `int` | Position mémorisée toutes les 3 frames — utilisée par le loup niv.2 pour calculer la vitesse du lapin |
| `dir_x, dir_y` | `int` | Direction mémorisée dans `deplacer_*()` — lue par les fonctions de dessin pour afficher la ligne directionnelle |
| `genre` | `int` | `Femelle(0)` ou `Male(1)` — tiré aléatoirement à la naissance |
| `energie` | `int` | Énergie courante — mort si ≤ 0 |
| `age` | `int` | Âge en frames — mort si > `Age_max` |
| `niveau` | `int` | Comportement (1 à 4) |
| `nb_echappements` | `int` | Seuils lapins : 1 → niv.2, 3 → niv.3, 6 → niv.4 |
| `nb_repas` | `int` | Seuils loups : 3 → niv.2, 6 → niv.3 |
| `frames_depuis_repro` | `int` | Délai depuis la dernière reproduction |
| `dans_vision_loup` | `bool` | État au tour précédent — la transition true→false détecte un échappement |
| `vivant` | `bool` | false = mort — compactage en fin de frame |
| `en_meute` | `bool` | true si ce loup fait partie d'une meute (niveau 4) |
| `etat` | `int` | `SAIN(0)`, `INFECTE(1)`, `RETABLI(2)` — modèle SIR |
| `timer_maladie` | `int` | Frames depuis infection ou guérison |

### `struct Fermier`
Zone de protection placée par le joueur au clic. Rayon fixe = 4 cases. Les loups dans le rayon fuient et perdent 2 énergie/frame.

### `struct Meteorite`
Événement physique à deux phases : cratère immédiat + onde de choc progressive par interpolation linéaire.

### `struct Monde`
Contient **tout l'état de la simulation**.

| Champ notable | Description |
|---------------|-------------|
| `grille[DIMW][DIMH]` | États : `vide(0)`, `herbe(1)`, `brule(2)`, `feu(3)`, `cendre(4)` |
| `timer_grille[DIMW][DIMH]` | Compteur parallèle pour les transitions brulé→vide et cendre→vide |
| `lapins[400]`, `loups[100]`, `vaches[50]` | Tableaux fixes, `nb_*` = nombre actifs |
| `frame` | Compteur global — affiché comme "Cycle" dans le panneau |
| `courbe` | Objet Plot Grapic pour les 5 courbes temps réel |
| `lv_N`, `lv_P` | Populations théoriques Lotka-Volterra (float, méthode Euler) |
| `saison` | `PRINTEMPS(0)`, `ETE(1)`, `AUTOMNE(2)`, `HIVER(3)` |
| `nb_infectes_lapins/loups/vaches` | Comptage séparé par espèce — affiché dans le panneau |
| `vitesse` | 1 à 10 — seuil de mise à jour = 11 - vitesse |

---

## Organisation des fonctions

### Utilitaires
| Fonction | Rôle |
|----------|------|
| `hasard(max)` | `rand() % max` |
| `chance(p)` | true avec probabilité p% |
| `valabs(a)` | Valeur absolue codée à la main |
| `dist_carre(x1,y1,x2,y2)` | Distance au carré — évite `sqrt()` |

### Cycle de vie dans `update()`
```
pousser_herbe()
  ↓
lapins : vieillir → déplacer → manger_herbe
reproduire_lapins()
  ↓
loups : vieillir → déplacer → manger_lapin (+ vaches)
reproduire_loups() → verifier_meutes()
  ↓
vaches : vieillir → déplacer → manger_herbe_vache
reproduire_vaches()
  ↓
update_meteorites() → update_incendie() → propager_maladie()
  ↓
compacter_lapins/loups/vaches()
  ↓
maj_niveaux() → maj_statistiques() → maj_historique()
frame++
```

### Algorithmes de direction

| Fonction | Utilisée par | Comportement |
|----------|-------------|--------------|
| `direction_vers_herbe()` | Lapins niv.1, vaches | Case HERBE la plus proche dans un rayon donné |
| `direction_fuite_aleatoire()` | Lapins niv.2 | Direction aléatoire en évitant les loups à distance 1 |
| `direction_fuite_intelligente()` | Lapins niv.3-4 | Maximise la distance au loup le plus proche (parmi 8 directions) |
| `direction_vers_lapin()` | Loups niv.1 | Lapin le plus proche dans un rayon |
| `direction_anticipee()` | Loups niv.2 | Prédit la position future du lapin via `px_avant` |
| `direction_vers_vache()` | Loups (fallback) | Vache la plus proche dans un rayon |
| `direction_fuite_feu()` | Tous | Fuit à l'opposé du centre de gravité des flammes proches |
| `direction_fuite_fermier()` | Loups | Fuit le fermier le plus proche |

---

## Dessin des animaux

Chaque animal est dessiné avec **deux appels Grapic** :

```cpp
// 1. Disque coloré selon le niveau
circleFill(px, py, taille);

// 2. Ligne blanche indiquant la direction
color(255, 255, 255);
line(px, py, px + dx*taille, py + dy*taille);
```

Un cercle supplémentaire est dessiné si l'animal est infecté (jaune) ou rétabli (cyan).

---

## Système de niveaux

### Lapins
| Niveau | Condition | Comportement |
|--------|-----------|-------------|
| 1 | Départ | Cherche l'herbe, ignore les loups |
| 2 | 1 échappement | Fuite aléatoire sécurisée |
| 3 | 3 échappements | Fuite optimisée (distance maximale) |
| 4 | 6 échappements | Idem 3 — seule une meute peut l'attraper |

### Loups
| Niveau | Condition | Comportement |
|--------|-----------|-------------|
| 1 | Départ | Cherche le lapin/vache le plus proche (rayon 3) |
| 2 | 3 repas | Anticipe la trajectoire du lapin |
| 3 | 6 repas | Se regroupe avec d'autres loups niv.3 |
| 4 meute | 2+ loups niv.3 proches | Rayon 6, peut manger tout lapin même niv.4 |

---

## Incendie — double buffer

```cpp
// On lit m.grille (état actuel) et on écrit dans nouvelle_grille (état futur)
int nouvelle_grille[DIMW][DIMH];
// copier l'état actuel dans nouvelle_grille
// lire m.grille pour décider, écrire dans nouvelle_grille
// recopier nouvelle_grille dans m.grille à la fin
```

Sans double buffer, le feu se propagerait à toute la grille en une seule frame (même problème que le Jeu de la Vie en TD9).

En été (`saison == ETE`), le bloc de propagation est exécuté deux fois → propagation doublée.

---

## Météorites — interpolation de rayon

```cpp
// Rayon de l'onde grandit de Meteorite_rayon_cratere à Meteorite_rayon_onde sur Meteorite_duree_onde frames
int rayon_actuel = Meteorite_rayon_cratere
    + (met.timer * (Meteorite_rayon_onde - Meteorite_rayon_cratere)) / Meteorite_duree_onde;
```

C'est une interpolation linéaire : à mi-chemin du timer, le rayon est à mi-chemin entre les deux valeurs.

---

## Contrôle de vitesse

```cpp
// Seuil de mise à jour = 11 - vitesse
// vitesse=1 → seuil=10 → update toutes les 10 frames (très lent)
// vitesse=10 → seuil=1 → update à chaque frame (très rapide)
compteur++;
if (!pause && compteur >= (11 - m.vitesse)) {
    update(m);
    compteur = 0;
}
```# Architecture du code

## Vue d'ensemble

Le projet est un fichier source unique `src/simulation.cpp` d'environ 2100 lignes, organisé en blocs thématiques. Toute la logique passe par un seul grand struct `Monde` passé par référence (`Monde& m`) à chaque fonction.

---

## Les structures de données

### `struct Animal`
Utilisé pour les **lapins, les loups et les vaches** — les trois espèces partagent le même struct.

| Champ | Type | Rôle |
|-------|------|------|
| `x, y` | `int` | Position sur la grille (en cases) |
| `px_avant, py_avant` | `int` | Position 3 frames avant (pour l'anticipation de trajectoire) |
| `dir_x, dir_y` | `int` | Direction mémorisée pour l'affichage (corrige le bug de rotation en pause) |
| `genre` | `int` | `Femelle(0)` ou `Male(1)` |
| `energie` | `int` | Énergie courante — mort si ≤ 0 |
| `age` | `int` | Âge en frames — mort si > `Age_max` |
| `niveau` | `int` | Comportement (1 à 4) |
| `nb_echappements` | `int` | Seuils lapins : 1 / 3 / 6 → passage niveau 2 / 3 / 4 |
| `nb_repas` | `int` | Seuils loups : 3 / 6 → passage niveau 2 / 3 |
| `frames_depuis_repro` | `int` | Délai depuis la dernière reproduction |
| `dans_vision_loup` | `bool` | État au tour précédent — détecte les échappements |
| `vivant` | `bool` | `false` = mort (compactage en fin de frame) |
| `en_meute` | `bool` | `true` si ce loup fait partie d'une meute (niveau 4) |
| `etat` | `int` | `SAIN / INFECTE / RETABLI` (modèle SIR) |
| `timer_maladie` | `int` | Frames depuis infection ou guérison |

### `struct Fermier`
Zone de protection placée par le joueur. Les loups dans le rayon fuient et perdent de l'énergie.

### `struct Meteorite`
Événement physique : cratère immédiat + onde de choc progressive.

### `struct Monde`
Contient **tout l'état de la simulation** : grille, tableaux d'animaux, courbes Plot, saison, fermiers, météorites.

---

## Organisation des fonctions

### Utilitaires
| Fonction | Rôle |
|----------|------|
| `hasard(max)` | `rand() % max` — entier aléatoire entre 0 et max-1 |
| `chance(p)` | Retourne `true` avec probabilité p% |
| `valabs(a)` | Valeur absolue codée à la main |
| `dist_carre(x1,y1,x2,y2)` | Distance au carré — évite `sqrt()` |

### Cycle de vie (appelé dans `update()`)
```
pousser_herbe()
  ↓
lapins : vieillir → déplacer → manger_herbe
reproduire_lapins()
  ↓
loups : vieillir → déplacer → manger_lapin (+ vaches)
reproduire_loups() → verifier_meutes()
  ↓
vaches : vieillir → déplacer → manger_herbe_vache
reproduire_vaches()
  ↓
update_meteorites() → update_incendie() → propager_maladie()
  ↓
compacter_lapins/loups/vaches()
  ↓
maj_niveaux() → maj_statistiques() → maj_historique()
frame++
```

### Algorithmes de direction

| Fonction | Utilisée par | Comportement |
|----------|-------------|--------------|
| `direction_vers_herbe()` | Lapins niv.1, vaches | Cherche la case HERBE la plus proche dans un rayon |
| `direction_fuite_aleatoire()` | Lapins niv.2 | Direction aléatoire qui évite les loups proches |
| `direction_fuite_intelligente()` | Lapins niv.3-4 | Maximise la distance au loup le plus proche |
| `direction_vers_lapin()` | Loups niv.1 | Cherche le lapin le plus proche dans un rayon |
| `direction_anticipee()` | Loups niv.2 | Prédit la position future du lapin (vitesse = pos - px_avant) |
| `direction_vers_vache()` | Loups (fallback) | Cherche la vache la plus proche |
| `direction_fuite_feu()` | Tous | Fuit dans la direction opposée au centre de gravité des flammes |
| `direction_fuite_fermier()` | Loups | S'éloigne du fermier le plus proche |

### Incendie — double buffer
```cpp
// On lit m.grille (état actuel) et on écrit dans nouvelle_grille (état futur)
// → une case qui vient de prendre feu ne propage pas dans la même frame
int nouvelle_grille[DIMW][DIMH];
// ... logique de propagation ...
for (int x...) for (int y...) m.grille[x][y] = nouvelle_grille[x][y];
```

---

## Système de niveaux

### Lapins
| Niveau | Condition | Comportement |
|--------|-----------|-------------|
| 1 | Départ | Va vers l'herbe, ignore les loups |
| 2 | 1 échappement | Fuite aléatoire sécurisée |
| 3 | 3 échappements | Fuite optimisée (maximise distance) |
| 4 | 6 échappements | Idem 3 — seule une meute peut l'attraper |

### Loups
| Niveau | Condition | Comportement |
|--------|-----------|-------------|
| 1 | Départ | Cherche le lapin/vache le plus proche (rayon 3) |
| 2 | 3 repas | Anticipe la trajectoire du lapin |
| 3 | 6 repas | Se regroupe avec d'autres loups niv.3 |
| 4 (meute) | 2+ loups niv.3 proches | Vision rayon 6, peut manger tout lapin |

---

## Bugs corrigés

### Triangles qui tournaient en pause
**Cause** : `dessiner_lapins()` recalculait la direction à chaque frame via `direction_vers_herbe()` qui contient `chance(50)`.  
**Fix** : ajout de `dir_x, dir_y` dans `struct Animal`, sauvegardé dans `deplacer_*()`, lu dans les fonctions de dessin.

### Pause qui se toggleait plusieurs fois
**Cause** : `isKeyPressed()` retourne `true` pendant toutes les frames où la touche est enfoncée.  
**Fix** : détection du front montant avec `bool espace_avant`.

### Feu qui brûlait toute la carte instantanément
**Cause** : lecture et écriture dans la même grille.  
**Fix** : double buffer — `nouvelle_grille` copie puis recopie.
