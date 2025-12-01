---
title: "Lògica dels bots RoBiT"
---

### *AIRoBiT_v1* Primera versió:
> Treu uns 4.000 pts de mitjana contra Dummys mentre que dummys treuen 1.300 pts aprox

Resultats de 15 partides:
| Jugador   | Nom       | Mitja Pts | 1r | 2n | 3r | 4t | Pos_Mitja | %Victoria |
|-----------|-----------|-----------|----|----|----|----|-----------|-----------|
| Jugador 1 | RoBiT_v1: | 4165      | 15 | 0  | 0  | 0  | 1.00      | 1.00      |
| Jugador 2 | Dummy:    | 1479      | 0  | 10 | 4  | 1  | 2.40      | 0.67      |
| Jugador 3 | Dummy:    | 1216      | 0  | 2  | 6  | 7  | 3.33      | 0.13      |
| Jugador 4 | Dummy:    | 1213      | 0  | 3  | 5  | 7  | 3.27      | 0.20      |

Segueix una estratègia de decisió molt bàsica, faig la millor acció possible per cada unitat individual (també conegut com a Greedy)

- Soldats (per ordre de prioritat):
    - Carreguen verí si no en tenen i tenen enemic a prop
    - Si tenen verí i enemic a prop ataquen
    - Si hi ha kit de verí aprop van a buscar-lo
    - Si no, busquen els diners/veri/vida que tinguin més propera

- Professor (per ordre de prioritat)
    - Si està ferit i té botiquins es cura
    - Obre la caixa forta si està davant
    - Busca la caixa forta més propera que no tingui un professor més a prop que ell.

### *AIRoBiT_v2*
> Si bé treu una mica menys (3.500pts de mitjana) contra només Dummys aconsegueix reduir la puntuació de l'enemic, Duumys treuen només uns 500-700 pts

Resultats de 15 partides:
| Jugador   | Nom       | Mitja Pts | 1r | 2n | 3r | 4t | Pos_Mitja | %Victoria |
|-----------|-----------|-----------|----|----|----|----|-----------|-----------|
| Jugador 1 | RoBiT_v1: | 1142      | 1  | 10 | 3  | 1  | 2.27      | 0.73      |
| Jugador 2 | RoBiT_v2: | 2991      | 14 | 1  | 0  | 0  | 1.07      | 1.00      |
| Jugador 3 | Dummy:    | 638       | 0  | 1  | 7  | 7  | 3.40      | 0.07      |
| Jugador 4 | Dummy:    | 704       | 0  | 3  | 5  | 7  | 3.27      | 0.20      |


- Introdueix concepte de prioritats, s'executa abans una acció d'atac que una de moviment.
- Només es mou el personatge amb l'acció més prioritària primer, si els altres tenen el mateix objectiu recalcula la jugada no tenint en compte aquell objectiu pels altres.

- Soldats (per ordre de prioritat):
    - Carreguen verí si no en tenen i tenen enemic a prop
    - Si tenen verí i enemic a prop ataquen:
        - Atac a distància 1 té prioritat màxima (vull atacar-lo abans)
        - Enemic a distància 2 té prioritat mínima (vull esperar al màxim ja que si ell es mou abans el mato jo)
    - Bloquegen caixes fortes que no són l'objectiu del professor
    - Busquen els diners/veri/vida que tinguin més propera i que sigui més necessària segons el moment, d’acord amb un sistema d’heurística.

### *AIRoBiT_v3*
> Apallissa a tothom (6.000 pts de mitjana contra Dummys) i guanya de forma consistent (tenint un temps d'execució molt més eficient) a RoBiT_v2

- Soldats milloren el bloqueig implementant sistema de d'heurística juntament amb diners/veri/vida:
    - pts_diners = dist_diners -2 (si quantitat >=15) -1 (si quantitat >= 10)
    - pts_veri = dist_veris -2 -3 (si quantitat veris disponible <5)
    - pts_vida = dist_vides +5 -3 (si available_health(me()) < professor_max_health()/2)
    - pts_bloqueig = dist_enemic_min - dist_soldat + 5;

- Professor
    - Calcula de forma correcta quina és la caixa més propera que no té cap enemic més proper que ell
    - Evita que soldats aliats li bloquegin la seva caixa objectiu

> Si algú es capaç d'igualar-ho de forma consistent el convido a una birra

Resultats de 15 partides:
| Jugador  | Nom      | Mitja Pts | 1r | 2n | 3r | 4t | Pos_Mitja | % Victoria |
|---------:|----------|----------:|---:|---:|---:|---:|----------:|-----------:|
| Jugador 1| RoBiT_v1 |      783  |  0 |  1 | 12 |  2 |     3.07  |      0.07  |
| Jugador 2| RoBiT_v2 |     1916  |  3 | 11 |  1 |  0 |     1.87  |      0.93  |
| Jugador 3| RoBiT_v3 |     4096  | 12 |  3 |  0 |  0 |     1.20  |      1.00  |
| Jugador 4| Dummy    |      512  |  0 |  0 |  2 | 13 |     3.87  |      0.00  |

#### v3_1 bloqueja els enemics que van cap a la mateixa caixa que el professor, mentre que v3_2 bloqueja els professors que van cap a caixes enemigues. No he tingut '.os' el suficientment bons com per provar quin dels dos és millor.