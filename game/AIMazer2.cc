#include "Player.hh"
#include <limits>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Mazer2


struct PLAYER_NAME : public Player {

    static Player* factory () {
        return new PLAYER_NAME;
    }

    //      CONSTANTS      //

    using VInt  = vector<int>;
    using VCela = vector<Cell>;
    using VDir  = vector<Dir>;
    using VPos  = vector<Pos>;

    using MInt  = vector<VInt>;
    using MCela = vector<VCela>;

    const VDir dirs = {Up, Down, Left, Right};
    const VInt x = {0, 0, -1, 1};
    const VInt y = {-1, 1, 0, 0};
    const int MAX = numeric_limits<int>::max();
    

    //      VARIABLES      //

    VPos pos_bazookas, pos_pistoles, pos_guerrers_enemics, pos_constructors_enemics, pos_diners, pos_menjar, pos_barricades;
    
    VInt meus_guerrers;
    VInt meus_constructors;

    MInt dist_bazookas;
    MInt dist_pistoles;
    MInt dist_guerrers_enemics;
    MInt dist_guerrers_enemics_guanyo;
    MInt dist_diners;
    MInt dist_menjar;
    MInt dist_barricades;

    //      CONSULTORES      //

    bool carrer(const Pos& p) {
        return cell(p).type == Street;
    }
    bool aliat(const Pos& p) {
        return cell(p).id != -1 and citizen(cell(p).id).player == me();
    }
    bool enemic(const Pos& p) {
        return cell(p).id != -1 and citizen(cell(p).id).player != me();
    }
    bool enemic_guerrer(const Pos& p) {
        return enemic(p) and citizen(cell(p).id).type == Warrior;
    }
    bool barricada_enemiga(const Pos& p) {
        return cell(p).b_owner != -1 and cell(p).b_owner != me();
    }
    bool cela_lliure(const Pos& p) {
        return pos_ok(p) and carrer(p) and cell(p).id == -1 and !barricada_enemiga(p);
    }


    /*  Omple mapa amb les distàncies relatives a les
        pos del vector pos_origen (per saber el path) */
    void buscar_mapa(VPos posicions_origen, MInt& mapa) {
        for (Pos pos_origen : posicions_origen) {
            queue<Pos> caselles_pendents;
            caselles_pendents.push(pos_origen);
            mapa[pos_origen.i][pos_origen.j] = 0;

            while(!caselles_pendents.empty()) {
                Pos pos_cela = caselles_pendents.front();
                caselles_pendents.pop();

                for (Dir d : dirs) {
                    Pos new_pos_cela = pos_cela + d;
                    int new_dist = mapa[pos_cela.i][pos_cela.j] + 1;
                    if (cela_lliure(new_pos_cela) and mapa[new_pos_cela.i][new_pos_cela.j] > new_dist) {
                        caselles_pendents.push(new_pos_cela);
                        mapa[new_pos_cela.i][new_pos_cela.j] = new_dist;
                    }
                }
            }
        }
    }

    /*  Retorna la direccio que mes s'apropa
        a l'objecte del mapa */
    Dir dir_mes_propera(const Pos& p, const MInt& mapa) {
        Dir dir_propera = Right;
        int min_dist = MAX;
        for (Dir d : dirs) {
            Pos next_p = p + d;
            if (pos_ok(next_p) and min_dist > mapa[next_p.i][next_p.j]) {
                dir_propera = d;
                min_dist = mapa[next_p.i][next_p.j];
            }
        }
        return dir_propera;
    }

    //COSA RARA
    Dir dir_mes_propera_2(const Pos& p, const MInt& mapa1, const MInt& mapa2) {
        Dir dir_propera = Up;
        int min_dist = MAX;
        for (Dir d : dirs) {
            Pos next_p = p + d;
            int new_dist = mapa1[next_p.i][next_p.j] + mapa2[next_p.i][next_p.j];
            if (cela_lliure(next_p) and min_dist > new_dist) {
                dir_propera = d;
                min_dist = new_dist;
            }
        }
        return dir_propera;
    }

    /* Fa el contrari a Dir_mes_propera,
    Retorna la direccio que més s'alluna a l'objecte del mapa */
    Dir fugir(const Pos& p, const MInt& mapa) {
        Dir dir_llunyana = Up;
        int max_dist = 0;
        for (Dir d : dirs) {
            Pos next_p = p + d;
            if (cela_lliure(next_p) and max_dist < mapa[next_p.i][next_p.j]) {
                dir_llunyana = d;
                max_dist = mapa[next_p.i][next_p.j];
            }
        }
        return dir_llunyana;
    }


    //      INICIALITZACIÓ      //

    void reset() {
        meus_guerrers = warriors(me());
        meus_constructors = builders(me());

        pos_bazookas.clear();
        pos_pistoles.clear();
        pos_guerrers_enemics.clear();
        pos_constructors_enemics.clear();
        pos_diners.clear();
        pos_menjar.clear();
        pos_barricades = barricades(me());

        dist_bazookas = dist_pistoles = dist_guerrers_enemics = dist_guerrers_enemics_guanyo =
        dist_diners = dist_menjar = dist_barricades = MInt(board_rows(), VInt(board_cols(), MAX));
    }

    void llegir_taulell() {

        for (int i = 0; i < board_rows(); ++i) {
            for (int j = 0; j < board_cols(); ++j) {
                Pos p = Pos({i, j});
                Cell casella = cell(p);
                if (casella.weapon == Bazooka) pos_bazookas.push_back(p);
                else if (casella.weapon == Gun) pos_pistoles.push_back(p);
                else if (enemic_guerrer(p)) pos_guerrers_enemics.push_back(p);
                else if (enemic(p)) pos_constructors_enemics.push_back(p);
                else if (casella.bonus == Money) pos_diners.push_back(p);
                else if (casella.bonus == Food) pos_menjar.push_back(p);
            }
        }

        buscar_mapa(pos_bazookas, dist_bazookas);
        buscar_mapa(pos_pistoles, dist_pistoles);
        buscar_mapa(pos_guerrers_enemics, dist_guerrers_enemics);
        buscar_mapa(pos_diners, dist_diners);
        buscar_mapa(pos_menjar, dist_menjar);
        buscar_mapa(pos_barricades, dist_barricades);
    }


    //      ACCIONS      //


    /*  Retorna True + direccio barricada/espai_lliure
        Si es ronda 20-25 i pot millorar/construir una barricada
        Si no pot retorna Fals */
    bool construeixo(const Pos& p, Dir& direccio) {
        if (is_day() and round()%25 > 20 and dist_diners[p.i][p.j] > 5) {
            for (Dir d : dirs) {
                if (cell(p+d).b_owner == me() and cell(p+d).resistance < barricade_max_resistance()) {
                    direccio = d;
                    return true;
                }
                else if (cela_lliure(p+d)) direccio = d;
            }
            if (int(pos_barricades.size()) < max_num_barricades()) return true;
        }
        return false;
    }

    /*  Retorna la direccio on és més optim moure's
        Ferit i dia --> Menjar/Barricades   (el més pròxim)
        Ferit, nit
            Enemic proper, Menjar/Barricades lluny --> Fugir
            Diners/Menjar                   (el més pròxim)
        Altre --> Diners */
    void direccio_optima(const Citizen& constructor, Dir& direccio) {
        Pos p = constructor.pos;

        direccio = dir_mes_propera(p, dist_diners);
        if (constructor.life < builder_ini_life()/2) {
            if (!is_day()) {
                if (dist_guerrers_enemics[p.i][p.j] < 5 and (dist_menjar[p.i][p.j] != 1 or dist_barricades[p.i][p.j] != 1)) fugir(p, dist_guerrers_enemics);
                else if (dist_menjar[p.i][p.j] < dist_barricades[p.i][p.j]) direccio = dir_mes_propera(p, dist_menjar);
                else direccio = dir_mes_propera(p, dist_barricades);
            }
            if (is_day() and dist_diners[p.i][p.j] > dist_menjar[p.i][p.j]) direccio = dir_mes_propera(p, dist_menjar);
        }
    }

    void possibles_atacs(const WeaponType& arma) {
        VPos pos_enemics_guanyo = pos_constructors_enemics;
        for (Pos p : pos_guerrers_enemics) {
        WeaponType arma_guerrer_enemic = citizen(cell(p).id).weapon;
            if (arma_guerrer_enemic != strongestWeapon(arma, arma_guerrer_enemic)) {
                pos_enemics_guanyo.push_back(p);
            }
        }
        buscar_mapa(pos_enemics_guanyo, dist_guerrers_enemics_guanyo);
    }

    Dir buscar_arma(const Pos& p, const WeaponType& arma_actual) {
        Dir dir_bazooka = dir_mes_propera(p, dist_bazookas);
        if (arma_actual == Gun and pos_bazookas.size() == 0) {
            return dir_mes_propera(p, dist_guerrers_enemics);
        }
        if (dist_bazookas[p.i][p.j] > dist_pistoles[p.i][p.j] and dist_pistoles[p.i][p.j] < 5) {
            return dir_mes_propera(p, dist_pistoles);
        }
        return dir_bazooka;
    }

    void guerrers() {
        for (int id : meus_guerrers) {
            Citizen guerrer = citizen(id);
            WeaponType arma_actual = guerrer.weapon;
            Pos p = guerrer.pos;
            Dir direccio;

            possibles_atacs(arma_actual);
            if (is_day()) {
                if (arma_actual != Bazooka) direccio = buscar_arma(p, arma_actual);
                else direccio = dir_mes_propera(p, dist_guerrers_enemics_guanyo);
            }
            else {
                if (arma_actual == Hammer) direccio = buscar_arma(p, arma_actual);
                else direccio = dir_mes_propera(p, dist_guerrers_enemics_guanyo);
            }
            move(id, direccio);
        }
    }

    void constructors() {
        for (int id : meus_constructors) {
            Citizen constructor = citizen(id);
            Pos p = constructor.pos;
            Dir direccio = Up;

            direccio = dir_mes_propera(p, dist_diners);
            move(id, direccio);
        }
    }

    virtual void play () {
        reset();
        llegir_taulell();

        guerrers();
        constructors();
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
