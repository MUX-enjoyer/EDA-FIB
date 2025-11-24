**AIRoBiT_v1** --> Primera versió:
Segueix una estratègia de decisió molt bàsica, tot i no exprimir el màxim l'informació que té a l'abast faig la millor acció possible per cada unitat individual (també conegut com a Greedy)

- Soldats (per ordre de prioritat):
    Carreguen verí si no en tenen i tenen enemic a prop
    Si tenen verí i enemic aprop ataquen
    Si hi ha kit_de verí aprop van a buscar-lo
    Sinó busquen els diners/veri/vida que tinguin més propera

- Professor (per ordre de prioritat)
    - Si està ferit i té botiquins es cura
    - Obre la caixa forta si està davant
    - Busca la caixa forta més propera que no tingui un professor més aprop que ell.

**AIRoBiT_v2**
- Introdueix concepte de prioritats, s'executa abans una acció d'atac que una de moviment.
- Només es mou el personatge amb l'acció més prioritaria primer, si els altres tenen el mateix objectiu recalcula la jugada no tenint en compte aquell objectiu per els altres.

- Soldats (per ordre de prioritat):
    Carreguen verí si no en tenen i tenen enemic a prop
    Si tenen verí i enemic aprop ataquen
    Guarrada màxima:
        - Atac a distància 1 té prioritat màxima (vull atacar-lo abans)
        - Enemic a distància 2 té prioritat mínima (vull esperar al màxim ja que si ell es mou abans el mato jo)
    Soldats bloquejen caixes fortes que no son l'objectiu del professor
    Busquen els diners/veri/vida que tinguin més propera i que sigui més necessària segons el moment segons un sistema d'heurística.
    
    
**Carelantis** --> Bot Aleix
