# Architecture du code

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
