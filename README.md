# Simulation Proies / Prédateurs — Lotka-Volterra

> Projet de fin de semestre — UE LIFAMI — Université Lyon 1 UCBL  
> **Ghrab Mohamed Adam** 

---

## Présentation

Simulation d'un écosystème proies/prédateurs inspirée du **modèle mathématique de Lotka-Volterra**.  
Trois espèces animales coexistent sur une grille 2D de 54 × 46 cases (DIMW × DIMH) :

| Espèce | Rôle | Représentation |
|--------|------|----------------|
| 🐇 Lapins | Proies — mangent l'herbe | Disque bleu → jaune selon le niveau |
| 🐺 Loups | Prédateurs — chassent lapins et vaches | Disque rouge → gris selon le niveau |
| 🐄 Vaches | Bétail — source d'énergie élevée pour les loups | Disque marron |

Chaque animal est représenté par un **disque coloré avec une ligne blanche** indiquant sa direction de déplacement. Les populations évoluent en temps réel. Les courbes simulées sont comparées aux courbes théoriques de Lotka-Volterra calculées par la méthode d'Euler.

---

## Fonctionnalités

- **Système de niveaux 1 à 4** pour lapins (via échappements) et loups (via repas)
- **Meutes de loups** : formation dynamique quand 3 loups niveau 3 se regroupent, dissolution si séparation
- **4 saisons** cliquables : modifient pousse de l'herbe, reproduction et vision des loups
- **Modèle SIR** : épidémie contagieuse avec comptage séparé par espèce (lapins / loups / vaches)
- **Fermiers** : zones de protection placées au clic (max 5, rayon 4 cases)
- **Météorites** : impact immédiat + onde de choc progressive avec interpolation de rayon
- **Incendies** : propagation cellulaire avec double buffer, fuite intelligente via centre de gravité des flammes
- **Courbes temps réel** : 5 courbes Plot (lapins sim., loups sim., lapins théoriques LV, loups théoriques LV, infectés)
- **Contrôle de vitesse** : touches + et 6 (vitesse 1 à 10)

---

## Contrôles

| Touche | Action |
|--------|--------|
| `ESPACE` | Pause / Reprendre |
| `R` | Réinitialiser la simulation |
| `F` + clic | Placer un fermier sur la grille (max 5) |
| `M` + clic | Lancer une météorite (max 3 simultanées) |
| `I` + clic | Démarrer un incendie |
| `+` | Augmenter la vitesse (max 10) |
| `6` | Diminuer la vitesse (min 1) |
| Boutons saisons | Changer la saison en cliquant dans le panneau droit |
| `ECHAP` | Quitter |

---

## Compilation

Ce projet utilise la bibliothèque **Grapic** de l'Université Lyon 1 (basée sur SDL2).

```bash
make
./simulation
```

> Grapic doit être installé selon les instructions de l'UE LIFAMI.  
> Documentation Grapic : https://alexandre.meyer.pages.univ-lyon1.fr/grapic/

---

## Structure du projet

```
LIFAMI-project/
├── src/
│   └── simulation.cpp       # Code source principal (~2100 lignes)
├── doc/
│   ├── ARCHITECTURE.md      # Explication des structs et fonctions
│   ├── MODELES_SCIENTIFIQUES.md  # Lotka-Volterra, SIR, automate cellulaire
│   └── CHANGELOG.md         # Historique des dépôts
├── .gitignore
├── Makefile
└── README.md
```

---

## Panneau de statistiques (droite)

Le panneau affiche en temps réel :
- Nombre de lapins, loups, vaches
- Numéro de **cycle** (frame)
- Saison active avec boutons cliquables
- Infectés total + détail par espèce séparé
- Vitesse de simulation
- Répartition par niveau (lapins niv.1 à 4, loups niv.1 à 4)
- Courbes Lotka-Volterra + infectés (en bas du panneau)

---

## Modèles scientifiques

### Lotka-Volterra
```
dN/dt = α·N − β·N·P      (proies)
dP/dt = δ·N·P − γ·P      (prédateurs)
```
Intégré par la **méthode d'Euler** (TD6-7 LIFAMI). Courbes théoriques superposées aux courbes simulées.

### Modèle SIR
```
SAIN → INFECTÉ (200 frames) → RÉTABLI (400 frames) → SAIN
```
Contagion par contact (rayon 2 cases, 8%/frame) et par prédation (100%).  
Les animaux infectés perdent 1 énergie supplémentaire par frame.

### Automate cellulaire — Incendie
Propagation case par case avec **double buffer** — même principe que le Jeu de la Vie (TD9 LIFAMI).  
Propagation doublée en été.

---

## Références

- Modèle de Lotka-Volterra : https://fr.wikipedia.org/wiki/Equations_de_Lotka-Volterra
- Modèle SIR : https://fr.wikipedia.org/wiki/Modeles_compartimentaux_en_epidemiologie
- Automate cellulaire : https://fr.wikipedia.org/wiki/Automate_cellulaire
- Bibliothèque Grapic Lyon 1 : https://alexandre.meyer.pages.univ-lyon1.fr/grapic/
- Inspiration comportements émergents : https://www.youtube.com/watch?v=qVOjXQUzOJw
