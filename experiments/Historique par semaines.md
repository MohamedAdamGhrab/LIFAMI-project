# Historique des évolutions — Semaine par semaine

> **Ghrab Mohamed Adam — p2406783 — UE LIFAMI — Lyon 1**  
> Mini-projet : Simulation Proies/Prédateurs — Lotka-Volterra

---

## Avant le mini-projet — Réflexion et conception

Avant même le début officiel du mini-projet, j'avais déjà réfléchi à l'architecture globale de la simulation. Les idées suivantes étaient déjà posées :

- Grille 2D avec herbe qui pousse aléatoirement
- Deux espèces en interaction (lapins proies, loups prédateurs)
- Système d'énergie : manger pour survivre, vieillir pour mourir
- Reproduction conditionnelle (énergie + délai + partenaire proche)
- Système de niveaux progressifs selon le comportement
- Idée de comparer la simulation aux équations de Lotka-Volterra

---

## Semaine 1 — Mise en place des bases

### Ce qui a été fait
- Création du `struct Animal` et du `struct Monde`
- Initialisation de la grille avec herbe aléatoire (65% de chances)
- Fonctions utilitaires : `hasard()`, `chance()`, `valabs()`
- `ajouter_lapin()` et `ajouter_loup()` avec initialisation complète de tous les champs
- `pousser_herbe()` : 5% de chance par case vide par frame
- `loup_voit_case()` et `lapin_vu_par_loup()` : champ de vision carré
- `direction_vers_herbe()` : le lapin cherche la case herbe la plus proche dans un rayon de 6
- `manger_herbe()`, `vieillir_lapin()`, `vieillir_loup()`
- `compacter_lapins()` et `compacter_loups()` : tassage des tableaux après morts
- Boucle principale avec pause (ESPACE) et reset (R)
- Affichage basique du panneau (Lapins, Loups, Frame)

### Problèmes rencontrés
- Comprendre pourquoi il ne faut pas supprimer immédiatement un animal mort pendant une boucle (indice sauté) → solution : marquer `vivant=false`, compacter à la fin

### Ce que j'avais prévu mais pas encore codé
- Le système de niveaux
- La reproduction
- Les meutes

---

## Semaine 2 — Niveaux, fuite, chasse

### Ce qui a été fait
- `monter_niveau_lapin()` : seuils 1/3/6 échappements → niveaux 2/3/4
- `monter_niveau_loup()` : seuils 3/6 repas → niveaux 2/3, vérification de meute pour niveau 4
- `verifier_meutes()` : dissolution de la meute si les loups s'éloignent
- `direction_fuite_aleatoire()` : lapin niveau 2, fuit dans une direction sans loup
- `direction_fuite_intelligente()` : lapin niveau 3-4, maximise la distance au loup le plus proche
- `direction_vers_lapin()` : loup niveau 1, cherche le plus proche
- `direction_anticipee()` : loup niveau 2, prédit la position future du lapin via `px_avant`/`py_avant`
- `deplacer_lapin()` et `deplacer_loup()` avec sélection du comportement selon le niveau
- `manger_lapin()` avec règles de prédation selon les niveaux (un lapin niv.4 ne peut être mangé que par une meute)
- `reproduire_lapins()` et `reproduire_loups()` avec contrôle de densité, délai, énergie minimale
- `maj_niveaux()` et `maj_historique()`

### Représentation : triangles
Les animaux sont représentés par des triangles pleins. Les fonctions `remplir_triangle()` et `dessiner_animal_plein()` ont été réécrites avec l'aide de l'IA car la rasterisation était complexe.

### Bug connu à ce stade
Les triangles tournent en pause car la direction est recalculée dans `dessiner_lapins()` via `direction_vers_herbe()` qui contient `chance(50)`. Ce bug sera corrigé dans le dépôt final.

### Dépôt officiel 1 soumis à la fin de cette semaine

---

## Semaine 3 — Correction des bugs et nouvelles espèces

### Corrections
- **Bug de rotation** : ajout de `dir_x`/`dir_y` dans `struct Animal`, sauvegardés dans `deplacer_*()` et lus dans les fonctions de dessin. La direction ne change plus quand la simulation est en pause.
- **Bug pause** : remplacement de `isKeyPressed(SDLK_SPACE)` direct par un front montant avec `bool espace_avant`. La pause ne se toggle plus plusieurs fois.
- **Mise en conformité** : suppression de `<string>` et `to_string()`. Tout l'affichage passe par `print(x, y, int)` directement (fonctions Grapic vues en TD).

### Nouvelles espèces et fonctionnalités
- `struct Animal` partagé pour les vaches : `ajouter_vache()`, `deplacer_vache()` (1 frame sur 2), `manger_herbe_vache()`, `vieillir_vache()`, `reproduire_vaches()`, `compacter_vaches()`
- Les loups peuvent aussi manger les vaches (100 énergie vs 60 pour un lapin)
- `direction_vers_vache()` ajoutée comme fallback pour les loups
- Vaches dessinées en marron avec la même fonction `dessiner_animal()`

---

## Semaine 4 — Saisons, SIR, Fermiers

### Saisons
- `taux_herbe(saison)` : PRINTEMPS=5%, ETE=8%, AUTOMNE=3%, HIVER=0%
- Effet sur `vieillir_lapin()` : perte d'énergie doublée en hiver
- Effet sur `loup_voit_case()` : rayon +1 en hiver
- Effet sur `reproduire_lapins()` : délai modifié selon la saison
- Boutons cliquables dans le panneau : `bouton()` avec détection de clic souris

### Modèle SIR
- Champ `etat` (SAIN/INFECTE/RETABLI) et `timer_maladie` ajoutés dans `struct Animal`
- `propager_maladie()` : infection spontanée (1%), contagion par contact (8%/rayon 2), transmission par prédation (100%)
- Guérison après 200 frames → immunité 400 frames → redevient sain
- Animaux infectés perdent 1 énergie supplémentaire par frame
- Affichage : cercle jaune (infecté), cercle cyan (rétabli)
- Comptage séparé : `nb_infectes_lapins`, `nb_infectes_loups`, `nb_infectes_vaches`

### Fermiers
- `struct Fermier` avec position, rayon (4 cases), actif
- `dans_rayon_fermier()` : protection des animaux dans la zone
- `direction_fuite_fermier()` : loup fuit et perd 2 énergie/frame
- Placement au clic après appui sur F
- Dessin : cercle bleu + point jaune au centre

---

## Semaine 5 — Météorites, Incendies, Courbes LV

### Météorites
- `struct Meteorite` avec timer et phase
- Phase 1 (timer==1) : cratère immédiat, `dist_carre` ≤ R² → BRULE, animaux → mort
- Phase 2 (timer 2..50) : onde de choc, rayon interpolé linéairement de 4 à 8 cases, `repousser_animal()` (-15 énergie)
- `dessiner_meteorites()` : animation d'impact puis cercle blanc progressif

### Incendies
- États supplémentaires dans la grille : FEU et CENDRE
- `update_incendie()` avec **double buffer** : `nouvelle_grille[DIMW][DIMH]` pour éviter la propagation instantanée (même principe que Jeu de la Vie, TD9)
- Propagation 15%/case voisine, doublée en été
- `pres_du_feu()` et `direction_fuite_feu()` : fuite vers l'opposé du centre de gravité des flammes
- Animation : couleur alternante orange/jaune toutes les 2 frames

### Courbes Lotka-Volterra
- Objet `Plot courbe` dans `struct Monde`
- `maj_historique()` : toutes les 10 frames, 5 courbes mises à jour
  - Courbes 0 et 1 : lapins et loups simulés
  - Courbes 2 et 3 : lapins et loups théoriques (méthode d'Euler, α=0.02, β=0.005, δ=0.002, γ=0.03, dt=0.5)
  - Courbe 4 : infectés total
- `plot_draw()` affiché en bas du panneau droit

### Vitesse dynamique
```cpp
// Seuil de mise à jour réglable en temps réel
if (!pause && compteur >= (11 - m.vitesse)) { update(m); compteur = 0; }
```
Touches + et - modifient `m.vitesse` entre 1 et 10.

### Dépôt final soumis à la fin de cette semaine

---

## Références utilisées

| Sujet | Source |
|-------|--------|
| Modèle de Lotka-Volterra | https://fr.wikipedia.org/wiki/Equations_de_Lotka-Volterra |
| Méthode d'Euler (intégration numérique) | TD6-7 LIFAMI + https://fr.wikipedia.org/wiki/Methode_d%27Euler |
| Modèle SIR (épidémiologie) | https://fr.wikipedia.org/wiki/Modeles_compartimentaux_en_epidemiologie |
| Automate cellulaire (incendie) | TD9 LIFAMI (Jeu de la Vie) + https://fr.wikipedia.org/wiki/Automate_cellulaire |
| Bibliothèque Grapic | https://alexandre.meyer.pages.univ-lyon1.fr/grapic/ |
| Comportements émergents | https://www.youtube.com/watch?v=qVOjXQUzOJw |
| Interpolation linéaire (météorite) | Cours de mathématiques L1 |
