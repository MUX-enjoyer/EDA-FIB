#include "Player.hh"
#include <vector>
using namespace std;

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME EnderWiggin

typedef vector<vector<int>> MatriuDistancies;
typedef vector<vector<Cell>> MatriuCaselles;
int dist_bazooka_moure_dia = 25;
int dist_bazooka_moure_nit = 15;
int dist_enemic_moure = 20;

struct PLAYER_NAME : public Player {

    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }

    /**
     * Types and attributes for your player can be defined here.
     */
    const vector<Dir> dirs = {Up, Down, Left, Right};
    const vector<int> x = {0, 0, -1, 1};
    const vector<int> y = {-1, 1, 0, 0};
    

    // Rep una cel·la, i mira si hi ha un guerrer meu
    bool tinc_guerrer(Cell c1, Citizen c2) {
        if (citizen(c1.id).player == c2.player) return true;
        else return false;
    }

    // BFS des de Posició pos_orignen
    // Omple mapa amb les distàncies des de pos_origen (per saber el path)
    void buscar_mapa(Pos pos_origen, MatriuDistancies& mapa) {
        queue<Pos> caselles_pendents;
        caselles_pendents.push(pos_origen);
        mapa[pos_origen.i][pos_origen.j] = 0;

        while(!caselles_pendents.empty()) {
            Pos pos_cela = caselles_pendents.front();
            caselles_pendents.pop();

            for (Dir d : dirs) {
                Pos new_pos_cela = pos_cela + d;
                Cell casella = cell(new_pos_cela);
                int new_dist = mapa[pos_cela.i][pos_cela.j] + 1;
                if (pos_ok(new_pos_cela) and casella.type == Street and (casella.id == -1 or citizen(casella.id).player == me()) and mapa[new_pos_cela.i][new_pos_cela.j] > new_dist and (casella.b_owner == me() or casella.b_owner == -1)) {
                    caselles_pendents.push(new_pos_cela);
                    mapa[new_pos_cela.i][new_pos_cela.j] = new_dist;
                }
            }
        }
    }

    void buscar_mapa_bazooka(vector<Pos> pos_bazookas, vector<int> guerrers) {
        MatriuDistancies dist_bazookas(board_rows(), vector<int>(board_cols(), 1000));

        for (Pos pos_bazooka : pos_bazookas) {
            buscar_mapa(pos_bazooka, dist_bazookas);
        }        

        for (int id : guerrers) {
            Pos pos_guerrer = citizen(id).pos;
            int dist_bazooka_guerrer = dist_bazookas[pos_guerrer.i][pos_guerrer.j];
            if ((is_day() and dist_bazooka_guerrer < dist_bazooka_moure_dia) or (!is_day() and dist_bazooka_guerrer and dist_bazooka_moure_nit)) {
                Dir dir_bazooka = Right;
                int min_dist_bazooka = 1000;
                for (int d = 0; d < 4; ++d) {
                    if (pos_ok(pos_guerrer.i + y[d], pos_guerrer.j + x[d]) and cell(pos_guerrer.i + y[d], pos_guerrer.j + x[d]).type == Street and min_dist_bazooka > dist_bazookas[pos_guerrer.i + y[d]][pos_guerrer.j + x[d]]) {
                        dir_bazooka = dirs[d];
                        min_dist_bazooka = dist_bazookas[pos_guerrer.i + y[d]][pos_guerrer.j + x[d]];
                    }
                }
                move(id, dir_bazooka);
            }
        }
    }

    void atacar(int id, vector<Pos>& pos_enemics) {
        MatriuDistancies dist_enemics(board_rows(), vector<int>(board_cols(), 1000));
        WeaponType arma_guerrer = citizen(id).weapon;


        for (Pos pos_enemic : pos_enemics) {
            WeaponType arma_enemic = citizen(cell(pos_enemic).id).weapon;
            if (arma_guerrer == strongestWeapon(arma_guerrer, arma_enemic)) buscar_mapa(pos_enemic, dist_enemics);
        }

        Pos pos_guerrer = citizen(id).pos;
        if (dist_enemics[pos_guerrer.i][pos_guerrer.j] < dist_enemic_moure) {
            Dir dir_enemic = Right;
            int min_dist_enemic = 1000;
            for (int d = 0; d < 4; ++d) {
                if (pos_ok(pos_guerrer.i + y[d], pos_guerrer.j + x[d]) and cell(pos_guerrer.i + y[d], pos_guerrer.j + x[d]).type == Street and min_dist_enemic > dist_enemics[pos_guerrer.i + y[d]][pos_guerrer.j + x[d]]) {
                    dir_enemic = dirs[d];
                    min_dist_enemic = dist_enemics[pos_guerrer.i + y[d]][pos_guerrer.j + x[d]];
                }
            }
            move(id, dir_enemic);
        }
    }


    /**
     * Play method, invoked once per each round.
     */
    virtual void play () {

        vector<Pos> pos_bazookas;
        vector<Pos> pos_enemics;
        vector<int> guerrers = warriors(me());
        vector<int> guerrers_buscar_bazooka;

        MatriuCaselles taulell(board_rows(), vector<Cell>(board_cols()));
        for (int i = 0; i < board_rows(); ++i) {
            for (int j = 0; j < board_cols(); ++j) {
                Cell casella = cell(i, j);
                if (casella.weapon == Bazooka) pos_bazookas.push_back({i,j});
                else if (casella.id != -1 and citizen(casella.id).player != me()) pos_enemics.push_back({i,j});
            }
        }
        
        for (int id_guerrer : guerrers) {
            Citizen guerrer = citizen(id_guerrer);
            if (guerrer.weapon != Bazooka) guerrers_buscar_bazooka.push_back(id_guerrer);
        }
        if (pos_bazookas.size() > 0) buscar_mapa_bazooka(pos_bazookas, guerrers_buscar_bazooka);

        //GUERRERS
        for (int id : guerrers) {
            Pos p = citizen(id).pos;
            int vida = citizen(id).life;
            enum WeaponType arma = citizen(id).weapon;
            enum WeaponType arma_enemic = NoWeapon;
            vector<Dir> dirs_possibles;
            Dir dir_menjar, dir_diners, dir_barricada, dir_arma, dir_enemic;
            dir_menjar = dir_diners = dir_barricada = dir_arma = dir_enemic = Up;
            bool menjar_aprop, diners_aprop, barricada_aprop, arma_aprop, enemic_aprop;
            menjar_aprop = diners_aprop = barricada_aprop = arma_aprop = enemic_aprop = false;

            // MIRO QUE TÉ A LES CASELLES ADJACENTS
            for (Dir d : dirs) {
                if (pos_ok(p+d)) {
                    if (cell(p+d).bonus == Food) {
                        menjar_aprop = true;
                        dir_menjar = d;
                    }
                    else if (cell(p+d).bonus == Money) {
                        diners_aprop = true;
                        dir_diners = d;
                    }
                    else if (cell(p+d).weapon != NoWeapon) {
                        if (!arma_aprop or strongestWeapon(cell(p+d).weapon, cell(p+dir_arma).weapon) == cell(p+d).weapon) dir_arma = d;
                        arma_aprop = true;
                    }
                    else if (cell(p+d).id != -1 and citizen(cell(p+d).id).player != me()) {
                        dir_enemic = d;
                        arma_enemic = citizen(cell(p+d).id).weapon;
                        enemic_aprop = true;
                    }
                    else if (cell(p+d).is_empty()) {
                        dirs_possibles.push_back(d);
                    }
                }
            }

            if (is_day()) { //DIA
                // ARMA(millor) -> VIDA(ferit) -> DINERS -> MENJAR -> BUSCA BAZOOKA
                if (arma_aprop) move(id, dir_arma);
                else if (vida < 100 and menjar_aprop) move(id, dir_menjar);
                else if (diners_aprop) move(id, dir_diners);
                else if (menjar_aprop) move(id, dir_menjar);
            }
            else { //NIT
                // ARMA(bazooka) -> LLUITAR(si millor) -> ARMA -> MENJAR(ferit) -> DINERS -> MENJAR -> RANDOM ||| FALTA DIR ENEMICS / BAZOOKA !!!
                if (arma_aprop and cell(p+dir_arma).weapon == Bazooka) move(id, dir_arma);
                if (enemic_aprop and !(strongestWeapon(arma, arma_enemic) == arma_enemic)) move(id, dir_enemic);
                else if (arma_aprop) move(id, dir_arma);
                else if (vida < 100 and menjar_aprop) move(id, dir_menjar);
                atacar(id, pos_enemics);    
            }
        }

        //CONSTRUCTORS
        vector<int> constructors = builders(me());
        for (int id : constructors) {
            Pos p = citizen(id).pos;
            int vida = citizen(id).life;
            int resistencia = 0;
            vector<Dir> dirs_possibles;
            Dir dir_menjar, dir_diners, dir_barricada, dir_arma, dir_buida;
            dir_menjar = dir_diners = dir_barricada = dir_arma = dir_buida = Up;
            bool menjar_aprop, diners_aprop, casella_buida, barricada_aprop, arma_aprop;
            menjar_aprop = diners_aprop = casella_buida = barricada_aprop = arma_aprop = false;
            
            // MIRO QUE TÉ A LES CASELLES ADJACENTS
            for (Dir d : dirs) {
                if (pos_ok(p+d)) {
                    if (cell(p+d).bonus == Food) {
                        menjar_aprop = true;
                        dir_menjar = d;
                    }
                    else if (cell(p+d).bonus == Money) {
                        diners_aprop = true;
                        dir_diners = d;
                    }
                    else if (cell(p+d).weapon != NoWeapon) {
                        arma_aprop = true;
                        dir_arma = d;
                    }
                    else if (cell(p+d).b_owner == me()) {
                        barricada_aprop = true;
                        dir_barricada = d;
                        resistencia = cell(p+d).resistance;
                    }
                    else if (cell(p+d).is_empty()) {
                        casella_buida = true;
                        dir_buida = d;
                    }
                    if (cell(p+d).is_empty() or cell(p+d).b_owner == me()) {
                        casella_buida = true;
                        dirs_possibles.push_back(d);
                    }
                }
            }
            if (is_day()) { //DIA
                // MENJAR(ferit) -> DINERS -> AMAGAR-SE(final ronda) -> MILLORAR -> ARMA -> BARRICADA -> MENJAR -> FALTA -> dir diners!
                if (vida < 100 and menjar_aprop) move(id, dir_menjar);
                else if (diners_aprop) move(id, dir_diners);
                else if (barricada_aprop and round()%50 > 23) move(id, dir_barricada);
                else if (barricada_aprop and resistencia < barricade_max_resistance())build(id, dir_barricada);
                else if (arma_aprop) move(id, dir_arma);
                else if (casella_buida and barricades(me()).size() < static_cast<size_t>(max_num_barricades())) build(id, dir_buida);
                else if (menjar_aprop) move(id, dir_menjar);
                else if (!dirs_possibles.empty()) move(id, dirs_possibles[random(0,dirs_possibles.size())]);
            }
            else { //NIT
                // MENJAR(ferit) -> AMAGAR-SE -> DINERS -> ARMA -> MENJAR -> RANDOM ||| FALTA -> dir barricada!
                if (vida < 100 and menjar_aprop) move(id, dir_menjar);
                else if (diners_aprop and vida >= 40) move(id, dir_diners);
                else if (barricada_aprop) move(id, dir_barricada);
                else if (diners_aprop) move(id, dir_diners);
                else if (arma_aprop) move(id, dir_arma);
                else if (menjar_aprop) move(id, dir_menjar);
                else if (!dirs_possibles.empty()) move(id, dirs_possibles[random(0,dirs_possibles.size())]);
            }
        }
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);