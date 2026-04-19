# Simulation Proies / Prédateurs — Lotka-Volterra

> Projet de fin de semestre — UE LIFAMI — Université Lyon 1 UCBL  
> **Ghrab Mohamed Adam** — p2406783

---

## Présentation

Simulation d'un écosystème proies/prédateurs inspirée du **modèle mathématique de Lotka-Volterra**.  
Trois espèces animales coexistent sur une grille 2D :

| Espèce | Rôle | Couleur |
|--------|------|---------|
| Lapins | Proies — mangent l'herbe | Bleu → Jaune selon le niveau |
| Loups | Prédateurs — chassent lapins et vaches | Rouge → Gris selon le niveau |
| Vaches | Bétail — source d'énergie élevée pour les loups | Marron |

Les populations évoluent en temps réel. Les courbes simulées sont comparées aux courbes théoriques de Lotka-Volterra calculées par la méthode d'Euler.

---

## Fonctionnalités

- **Système de niveaux progressifs** : lapins et loups évoluent selon leurs actions (échappements / repas)
- **Meutes de loups** : formation dynamique quand 3 loups niveau 3 se regroupent
- **4 saisons** : modifient la pousse de l'herbe, la reproduction et la vision
- **Modèle SIR** : épidémie contagieuse entre animaux avec immunité temporaire
- **Fermiers** : zones de protection placées par le joueur
- **Météorites** : impact + onde de choc physique sur les animaux
- **Incendies** : propagation cellulaire avec double buffer (comme le Jeu de la Vie)
- **Courbes temps réel** : 5 courbes (simulées + théoriques LV + infectés)

---

## Contrôles

| Touche | Action |
|--------|--------|
| `ESPACE` | Pause / Reprendre |
| `R` | Réinitialiser la simulation |
| `F` + clic | Placer un fermier (max 5) |
| `M` + clic | Lancer une météorite (max 3) |
| `I` + clic | Démarrer un incendie |
| `+` / `-` | Augmenter / Diminuer la vitesse (1 à 10) |
| Boutons saisons | Changer la saison dans le panneau |
| `ECHAP` | Quitter |

---

## Compilation

Ce projet utilise la bibliothèque **Grapic** de l'Université Lyon 1 (basée sur SDL2).

```bash
make
./simulation
```

> Grapic doit être installé et configuré selon les instructions de l'UE LIFAMI.  
> Documentation Grapic : https://alexandre.meyer.pages.univ-lyon1.fr/grapic/

---

## Structure du projet

```
LIFAMI-project/
├── src/
│   └── simulation.cpp       # Code source principal (~1200 lignes)
├── doc/
│   ├── ARCHITECTURE.md      # Explication des structs et fonctions
│   ├── MODELES_SCIENTIFIQUES.md  # Lotka-Volterra et SIR
│   └── CHANGELOG.md         # Historique des dépôts
├── .gitignore
├── Makefile
└── README.md
```

---

## Modèles scientifiques

### Lotka-Volterra
```
dN/dt = α·N − β·N·P      (proies)
dP/dt = δ·N·P − γ·P      (prédateurs)
```
Intégré numériquement par la **méthode d'Euler** (vue en TD LIFAMI).  
Les courbes théoriques sont superposées aux courbes simulées en temps réel.

### Modèle SIR
```
SAIN → INFECTÉ → RÉTABLI → SAIN
```
Épidémie contagieuse par proximité (rayon 2 cases, 8% de chance par frame).

---

## Références

- Modèle de Lotka-Volterra : https://fr.wikipedia.org/wiki/Equations_de_Lotka-Volterra
- Modèle SIR : https://fr.wikipedia.org/wiki/Modeles_compartimentaux_en_epidemiologie
- Automate cellulaire (incendie) : https://fr.wikipedia.org/wiki/Automate_cellulaire
- Bibliothèque Grapic Lyon 1 : https://alexandre.meyer.pages.univ-lyon1.fr/grapic/
- Inspiration comportements émergents : https://www.youtube.com/watch?v=qVOjXQUzOJw
