# Modèles scientifiques

---

## 1. Lotka-Volterra

### Équations
```
dN/dt = α·N − β·N·P      (proies)
dP/dt = δ·N·P − γ·P      (prédateurs)
```

| Paramètre | Valeur | Signification |
|-----------|--------|---------------|
| α = 0.02 | — | Taux de natalité des proies |
| β = 0.005 | — | Taux de prédation |
| δ = 0.002 | — | Taux de croissance des prédateurs par repas |
| γ = 0.03 | — | Taux de mortalité naturelle des prédateurs |
| dt = 0.5 | — | Pas de temps pour l'intégration d'Euler |

### Méthode d'Euler (TD6-7 LIFAMI)
```cpp
float dN = alpha*m.lv_N - beta*m.lv_N*m.lv_P;
float dP = delta*m.lv_N*m.lv_P - gamma*m.lv_P;
m.lv_N += dN * dt;
m.lv_P += dP * dt;
```
Toutes les 10 frames, ces valeurs sont ajoutées aux courbes (index 2 et 3 du Plot).

### 5 courbes Plot
| Index | Couleur | Contenu |
|-------|---------|---------|
| 0 | Bleu | Lapins simulés |
| 1 | Rouge | Loups simulés |
| 2 | Cyan | Lapins théoriques LV (Euler) |
| 3 | Orange | Loups théoriques LV (Euler) |
| 4 | Jaune | Infectés total |

---

## 2. Modèle SIR

### Transitions
```
SAIN(0) ──[1%/frame spontané ou 8% par contact]──▶ INFECTE(1)
INFECTE(1) ──[après 200 frames]──▶ RETABLI(2)
RETABLI(2) ──[après 400 frames]──▶ SAIN(0)
```

### Paramètres
| Constante | Valeur | Rôle |
|-----------|--------|------|
| `Proba_infection_ini` | 1% | Infection spontanée par frame (si population > 20) |
| `Proba_contagion` | 8% | Transmission par contact à distance ≤ 2 cases |
| `DUREE_Infection` | 200 frames | Durée de la phase infectée |
| `DUREE_Immunite` | 400 frames | Durée de l'immunité (phase RETABLI) |

### Effets
- Animal infecté : perd 1 énergie supplémentaire par frame
- Transmission par prédation : 100% (manger une proie infectée = tomber malade)
- Cercle **jaune** = infecté, cercle **cyan** = rétabli immunisé

---

## 3. Automate cellulaire — Incendie

### Règles de transition (par frame)
```
HERBE + voisin FEU + chance(15%) → FEU
FEU après 20 frames → CENDRE
CENDRE après 300 frames → VIDE
VIDE + chance(taux_saison%) → HERBE
```

### Double buffer
Lecture dans `m.grille`, écriture dans `nouvelle_grille`, recopie à la fin. Sans ça, une case qui vient de prendre feu propagerait immédiatement dans la même frame.

En été (`saison == ETE`), le bloc de propagation est exécuté deux fois → probabilité effective doublée.

### Météorites
```
timer == 1 : cratère immédiat (dist_carre ≤ R²) → BRULE, animaux → mort
timer 2..50 : rayon_actuel = R_cratere + timer*(R_onde - R_cratere)/duree_onde
              → interpolation linéaire → repousser_animal() dans l'anneau
timer > 80 : meteorite.actif = false
```

`dist_carre` évite `sqrt()` : comparer `dist² ≤ R²` est mathématiquement équivalent mais plus rapide.
