# Modèles scientifiques

Ce projet implémente et compare **trois modèles scientifiques** :

---

## 1. Modèle de Lotka-Volterra

### Les équations
Le modèle de Lotka-Volterra décrit la dynamique de deux populations en interaction (proies N et prédateurs P) :

```
dN/dt = α·N − β·N·P
dP/dt = δ·N·P − γ·P
```

| Paramètre | Valeur | Signification |
|-----------|--------|---------------|
| α = 0.02 | — | Taux de natalité des proies |
| β = 0.005 | — | Taux de prédation |
| δ = 0.002 | — | Taux de croissance des prédateurs par repas |
| γ = 0.03 | — | Taux de mortalité naturelle des prédateurs |
| dt = 0.5 | — | Pas de temps pour l'intégration d'Euler |

### Intégration numérique — méthode d'Euler
Les équations différentielles continues sont intégrées numériquement :

```
N(t+dt) = N(t) + dN/dt × dt
P(t+dt) = P(t) + dP/dt × dt
```

Implémentation dans `maj_historique()` :
```cpp
float dN = alpha*m.lv_N - beta*m.lv_N*m.lv_P;
float dP = delta*m.lv_N*m.lv_P - gamma*m.lv_P;
m.lv_N += dN * dt;
m.lv_P += dP * dt;
```

### Comparaison simulation / théorie
Toutes les 10 frames, 4 courbes sont tracées sur le Plot Grapic :

| Index | Courbe | Description |
|-------|--------|-------------|
| 0 | Lapins simulés (bleu) | Nombre réel de lapins dans la simulation |
| 1 | Loups simulés (rouge) | Nombre réel de loups dans la simulation |
| 2 | Lapins théoriques (vert) | Prédiction Lotka-Volterra par Euler |
| 3 | Loups théoriques (orange) | Prédiction Lotka-Volterra par Euler |
| 4 | Infectés (jaune) | Nombre total d'animaux infectés |

La comparaison permet d'observer l'impact des comportements émergents (niveaux, meutes, saisons, catastrophes) sur l'équilibre prédit par le modèle mathématique pur.

### Comportement attendu
Les populations oscillent en phase décalée : quand les lapins sont nombreux, les loups se multiplient → les lapins diminuent → les loups meurent de faim → les lapins se reconstituent → cycle recommence.

---

## 2. Modèle SIR (épidémie)

### Les transitions
```
SAIN(0) ──[infection]──▶ INFECTÉ(1) ──[guérison]──▶ RÉTABLI(2) ──[fin immunité]──▶ SAIN(0)
```

### Paramètres
| Constante | Valeur | Signification |
|-----------|--------|---------------|
| `Proba_infection_ini` | 1% | Infection spontanée par frame (si population > 20) |
| `Proba_contagion` | 8% | Transmission par contact (rayon 2 cases) par frame |
| `DUREE_Infection` | 200 frames | Durée de la phase infectée |
| `DUREE_Immunite` | 400 frames | Durée de la phase rétablie (immunisé) |

### Mécanismes de transmission
1. **Spontanée** : 1% de chance par frame qu'un lapin sain tombe malade
2. **Par contact** : un infecté contamine ses voisins dans un rayon de 2 cases (8%/frame)
3. **Par prédation** : un loup qui mange une proie infectée tombe malade (100%)

### Effets
- Un animal infecté perd **1 énergie supplémentaire** par frame
- Cercle **jaune** autour des infectés, **cyan** autour des rétablis
- Les rétablis sont immunisés pendant 400 frames avant de redevenir vulnérables

### Connexion avec Lotka-Volterra
Une épidémie qui frappe les lapins fait chuter leur population brutalement → les loups affamés meurent → les courbes simulées divergent des courbes théoriques Lotka-Volterra qui ne modélisent pas la maladie.

---

## 3. Automate cellulaire — Incendie

### Règles de transition
Chaque case FEU, à chaque frame :
- Tue tout animal présent sur la case
- Propage avec **15% de chance** aux cases HERBE voisines (×2 en été)
- Après 20 frames → devient CENDRE
- CENDRE → VIDE après 300 frames

### Double buffer
Pour éviter que le feu ne se propage instantanément à toute la carte :

```
Lire état actuel (m.grille)
       ↓
Écrire dans une copie (nouvelle_grille)
       ↓
Recopier la copie dans m.grille
```

C'est le même principe que le **Jeu de la Vie** vu en TD LIFAMI : chaque case est évaluée avec l'état du tour précédent.

---

## Références

- Equations de Lotka-Volterra : Lotka (1910), Volterra (1926)
- Modèle SIR : Kermack & McKendrick (1927)
- Méthode d'Euler : intégration numérique vue en TD6-7 LIFAMI
- Automate cellulaire : Conway (1970), TD9 LIFAMI
