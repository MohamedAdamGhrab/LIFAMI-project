# Changelog

Historique complet des dépôts et évolutions du projet.

---

## Dépôt final — version actuelle (~2100 lignes)

### Ajouts majeurs
- **Vaches** : troisième espèce, se déplace une frame sur deux (plus lente), donne 100 énergie aux loups (vs 60 pour un lapin)
- **Fermiers** : zones de protection placées au clic (max 5), rayon 4 cases — loups dans le rayon fuient et perdent 2 énergie/frame
- **Météorites** : deux phases — cratère immédiat (rayon 4, cases → BRULE, animaux → mort) puis onde de choc progressive par interpolation linéaire (rayon 4→8 sur 50 frames, repousse les animaux de 2 cases, -15 énergie)
- **Incendies** : propagation cellulaire avec double buffer (FEU → CENDRE → VIDE), fuite intelligente via centre de gravité des flammes, propagation doublée en été
- **4 saisons avec boutons cliquables** : PRINTEMPS (5%), ETE (8%, propagation feu ×2), AUTOMNE (3%), HIVER (0%, vision loups +1, lapins perdent 2 énergie/frame)
- **Modèle SIR** : épidémie avec comptage séparé par espèce (`nb_infectes_lapins`, `nb_infectes_loups`, `nb_infectes_vaches`) affiché dans le panneau
- **Courbes Lotka-Volterra** : 5 courbes temps réel — lapins/loups simulés + lapins/loups théoriques (Euler) + infectés
- **Contrôle de vitesse** : touche `+` pour accélérer, `6` pour ralentir (formule : seuil = 11 - vitesse)
- **Représentation des animaux** : disques colorés avec ligne blanche de direction (`circleFill` + `line`), cercle jaune pour infectés, cercle cyan pour rétablis

### Corrections de bugs
- **Direction stable en pause** : `dir_x`/`dir_y` sauvegardés dans `deplacer_*()` et lus dans les fonctions de dessin — la direction ne change plus quand la simulation est en pause
- **Pause fiable** : détection du front montant avec `bool espace_avant` — un seul toggle par appui
- **Feu non instantané** : double buffer sur la grille — le feu ne propage plus à toute la carte en une frame

---

## Dépôt 2 — version intermédiaire

### Ajouts
- Système de **niveaux 1 à 4** pour les lapins (échappements) et les loups (repas)
- **Meutes de loups** (niveau 4) : formation dynamique si 2+ loups niveau 3 dans un rayon de 3 cases, dissolution si séparation (`verifier_meutes()` à chaque frame)
- **Anticipation de trajectoire** pour les loups niveau 2 — calcul de la vitesse via `px_avant`/`py_avant` mis à jour toutes les 3 frames
- **Fuite intelligente** pour les lapins niveau 3-4 — maximise la distance au loup le plus proche parmi 8 directions
- Reproduction des loups et des vaches
- `case_voisine_libre()` vérifie que la case n'est pas occupée avant de placer un nouveau-né

---

## Dépôt 1 — mise en place initiale

### Mise en place
- Grille 2D `DIMW × DIMH` avec herbe qui pousse aléatoirement (5%/frame)
- `struct Animal`, `struct Monde`
- Lapins : déplacement vers l'herbe, énergie, mort par famine et vieillesse, reproduction avec contrôle de densité
- Loups : déplacement vers les lapins, prédation, énergie, reproduction
- **Compactage** des morts en fin de frame (marquage `vivant=false` + tassage du tableau)
- Panneau d'affichage avec statistiques de base
- Contrôle de vitesse par compteur
- Touches ESPACE (pause) et R (reset)

---

## Tableau des TDs LIFAMI utilisés

| Concept | TD LIFAMI | Application dans ce projet |
|---------|----------|---------------------------|
| Tableaux 2D | TD2-3 | `grille[DIMW][DIMH]`, `timer_grille[DIMW][DIMH]` |
| Structs | TD4 | `Animal`, `Fermier`, `Meteorite`, `Monde` |
| Fonctions & références | TD5 | `Monde &m` passé partout par référence |
| Équations différentielles / Euler | TD6-7 | Courbes Lotka-Volterra dans `maj_historique()` |
| Automate cellulaire | TD9 (Jeu de la Vie) | Incendie avec double buffer dans `update_incendie()` |
| Probabilités / rand() | TD1 | `hasard()`, `chance()` utilisés partout |
| Grapic / SDL2 | TD3+ | `circleFill`, `line`, `plot_add`, `plot_draw`, `print`, `bouton()` |
