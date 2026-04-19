# Changelog

---

## Version finale (~2100 lignes)

### Ajouts
- **Vaches** : 3e espèce, se déplace 1 frame sur 2, donne 100 énergie aux loups (vs 60 pour un lapin)
- **Fermiers** : zones de protection cliquables (max 5, rayon 4 cases) — loups dans le rayon fuient et perdent 2 énergie/frame
- **Météorites** : cratère immédiat (`dist_carre`) + onde de choc par interpolation linéaire (rayon 4→8 sur 50 frames)
- **Incendies** : double buffer, propagation doublée en été, fuite via centre de gravité des flammes
- **4 saisons** avec boutons cliquables — modifient herbe, reproduction, vision, énergie
- **Modèle SIR** avec comptage séparé `nb_infectes_lapins/loups/vaches` dans le panneau
- **5 courbes Plot** temps réel (lapins/loups simulés + théoriques LV + infectés)
- **Vitesse dynamique** : seuil = `11 - m.vitesse`, touches + et 6

### Corrections
- **Direction stable en pause** : `dir_x/dir_y` sauvegardés dans `deplacer_*()`, lus dans le dessin
- **Pause fiable** : front montant avec `bool espace_avant`
- **Feu non instantané** : double buffer sur la grille
- **Affichage conforme TD/TP** : `print(x, y, int)` au lieu de `<string>` + `to_string()`

---

## Dépôt 1 (~900 lignes)

### Mise en place
- Grille `DIMW × DIMH`, herbe aléatoire (65%), pousse à 5%/frame
- `struct Animal` (lapins + loups), `struct Monde`
- Lapins : niv.1 à 4 via échappements, fuite aléatoire/intelligente
- Loups : niv.1 à 4 via repas, meutes, anticipation de trajectoire
- Reproduction avec contrôle de densité, compactage des morts
- Panneau basique (Frame, Lapins, Loups, niveaux)
- Touches ESPACE/R

### Bugs connus à ce stade
- Triangles tournant en pause (direction recalculée dans le dessin)
- Pause se toggleant plusieurs fois (`isKeyPressed` sans front montant)

---

## Tableau des TDs LIFAMI utilisés

| Concept | TD | Utilisation dans le projet |
|---------|----|-----------------------------|
| Tableaux 2D | TD2-3 | `grille[DIMW][DIMH]`, `timer_grille` |
| Structs | TD4 | `Animal`, `Fermier`, `Meteorite`, `Monde` |
| Fonctions & références | TD5 | `Monde &m` dans toutes les fonctions |
| Équations différentielles / Euler | TD6-7 | `maj_historique()` — courbes LV |
| Automate cellulaire | TD9 (Jeu de la Vie) | `update_incendie()` — double buffer |
| Probabilités / rand() | TD1 | `hasard()`, `chance()` |
| Grapic | TD3+ | `circleFill`, `line`, `plot_add`, `print`, `bouton()` |
