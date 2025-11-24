### *AIRoBiT_v1* Primera versió:
> Treu uns 4.000pts de mitjana contra Dummys mentre que dummys treuen 1.300pts aprox

Resultats de 15 partides:
| Jugador   | Nom       | Mitja Pts | 1r | 2n | 3r | 4t | Pos_Mitja | %Victoria |
|-----------|-----------|-----------|----|----|----|----|-----------|-----------|
| Jugador 1 | RoBiT_v1: | 4165      | 15 | 0  | 0  | 0  | 1.00      | 1.00      |
| Jugador 2 | Dummy:    | 1479      | 0  | 10 | 4  | 1  | 2.40      | 0.67      |
| Jugador 3 | Dummy:    | 1216      | 0  | 2  | 6  | 7  | 3.33      | 0.13      |
| Jugador 4 | Dummy:    | 1213      | 0  | 3  | 5  | 7  | 3.27      | 0.20      |

Segueix una estratègia de decisió molt bàsica, tot i no exprimir el màxim l'informació que té a l'abast faig la millor acció possible per cada unitat individual (també conegut com a Greedy)

- Soldats (per ordre de prioritat):
    - Carreguen verí si no en tenen i tenen enemic a prop
    - Si tenen verí i enemic aprop ataquen
    - Si hi ha kit_de verí aprop van a buscar-lo
    - Sinó busquen els diners/veri/vida que tinguin més propera

- Professor (per ordre de prioritat)
    - Si està ferit i té botiquins es cura
    - Obre la caixa forta si està davant
    - Busca la caixa forta més propera que no tingui un professor més aprop que ell.

### *AIRoBiT_v2*
> Si bé treu una mica menys (3.500pts de mitjana) contra només Dummys aconsegueix redueïr la puntuació de l'enemic, Duumys treuen només uns 500-700s

Resultats de 15 partides:
| Jugador   | Nom       | Mitja Pts | 1r | 2n | 3r | 4t | Pos_Mitja | %Victoria |
|-----------|-----------|-----------|----|----|----|----|-----------|-----------|
| Jugador 1 | RoBiT_v1: | 1142      | 1  | 10 | 3  | 1  | 2.27      | 0.73      |
| Jugador 2 | RoBiT_v2: | 2991      | 14 | 1  | 0  | 0  | 1.07      | 1.00      |
| Jugador 3 | Dummy:    | 638       | 0  | 1  | 7  | 7  | 3.40      | 0.07      |
| Jugador 4 | Dummy:    | 704       | 0  | 3  | 5  | 7  | 3.27      | 0.20      |


- Introdueix concepte de prioritats, s'executa abans una acció d'atac que una de moviment.
- Només es mou el personatge amb l'acció més prioritaria primer, si els altres tenen el mateix objectiu recalcula la jugada no tenint en compte aquell objectiu per els altres.

- Soldats (per ordre de prioritat):
    - Carreguen verí si no en tenen i tenen enemic a prop
    - Si tenen verí i enemic aprop ataquen:
        - Atac a distància 1 té prioritat màxima (vull atacar-lo abans)
        - Enemic a distància 2 té prioritat mínima (vull esperar al màxim ja que si ell es mou abans el mato jo)
    - Bloquejen caixes fortes que no son l'objectiu del professor
    - Busquen els diners/veri/vida que tinguin més propera i que sigui més necessària segons el moment segons un sistema d'heurística.
    
    
### *Carelantis* Bot Aleix
Resultats de 15 partides:
| Jugador   | Nom          | Mitja Pts | 1r | 2n | 3r | 4t | Pos_Mitja | %Victoria |
|-----------|--------------|-----------|----|----|----|----|-----------|-----------|
| Jugador 1 | RoBiT_v1:    | 874       | 0  | 1  | 13 | 1  | 3.00      | 0.07      |
| Jugador 2 | RoBiT_v2:    | 3111      | 12 | 2  | 1  | 0  | 1.27      | 0.93      |
| Jugador 3 | Carelantis:  | 2250      | 3  | 12 | 0  | 0  | 1.80      | 1.00      |
| Jugador 4 | Dummy:       | 528       | 0  | 0  | 1  | 14 | 3.93      | 0.00      |