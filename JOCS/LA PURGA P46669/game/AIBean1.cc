#include "Player.hh"
#include <limits>
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Bean1


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
        
    struct Accio {
        int id;
        Dir dir;
        int prioritat;

        bool operator>(const Accio& a) const {
            return prioritat > a.prioritat;
        }

        Accio() : id(-1), dir(Up), prioritat(0) {}
        Accio(int id, Dir dir, int prio) : id(id), dir(dir), prioritat(prio) {}
    };

    //      VARIABLES      //

    VPos pos_bazookas, pos_pistoles, pos_guerrers_enemics, pos_constructors_enemics, pos_diners, pos_menjar, pos_barricades;
    
    VInt meus_guerrers;
    VInt meus_constructors;

    MInt dist_bazookas;
    MInt dist_armes;
    MInt dist_guerrers_enemics;
    MInt dist_enemics_guanyo;
    MInt dist_enemics_guanyo_bazookas;
    MInt dist_diners;
    MInt dist_diners_menjar;
    MInt dist_barricades;

    priority_queue<Accio, vector<Accio>, greater<Accio>> accions;
    int prio;

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

    void fer_accions() {
        while(!accions.empty()) {
            Accio a = accions.top();
            accions.pop();
            move(a.id, a.dir);
        }
    }



    /*  Omple mapa amb les distàncies relatives a les
        pos del vector pos_origen (per saber el path) */
    void buscar_mapa(VPos posicions_origen, MInt& mapa, int multi) {
        for (Pos pos_origen : posicions_origen) {
            queue<Pos> caselles_pendents;
            caselles_pendents.push(pos_origen);
            mapa[pos_origen.i][pos_origen.j] = 0;

            while(!caselles_pendents.empty()) {
                Pos pos_cela = caselles_pendents.front();
                caselles_pendents.pop();

                for (Dir d : dirs) {
                    Pos new_pos_cela = pos_cela + d;
                    int new_dist = mapa[pos_cela.i][pos_cela.j] + multi;
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

        dist_bazookas = dist_armes = dist_guerrers_enemics = dist_enemics_guanyo = dist_enemics_guanyo_bazookas =
        dist_diners = dist_diners_menjar = dist_barricades = MInt(board_rows(), VInt(board_cols(), MAX));

        prio = 1000;
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

        buscar_mapa(pos_bazookas, dist_bazookas, 1);
        buscar_mapa(pos_bazookas, dist_armes, 1);
        buscar_mapa(pos_bazookas, dist_enemics_guanyo_bazookas, 1);
        buscar_mapa(pos_pistoles, dist_armes, 2);
        buscar_mapa(pos_guerrers_enemics, dist_guerrers_enemics, 1);
        buscar_mapa(pos_diners, dist_diners, 1);
        buscar_mapa(pos_menjar, dist_diners_menjar, 1);
    }


    //      ACCIONS      //

    void possibles_atacs(const WeaponType& arma) {
        VPos pos_enemics_guanyo = pos_constructors_enemics;
        for (Pos p : pos_guerrers_enemics) {
        WeaponType arma_guerrer_enemic = citizen(cell(p).id).weapon;
            if (arma_guerrer_enemic != strongestWeapon(arma, arma_guerrer_enemic)) {
                pos_enemics_guanyo.push_back(p);
            }
        }
        buscar_mapa(pos_enemics_guanyo, dist_enemics_guanyo, 1);
        buscar_mapa(pos_enemics_guanyo, dist_enemics_guanyo_bazookas, 3);
    }

    Dir buscar_arma(const Pos& p, const WeaponType& arma_actual) {
        Dir dir_arma = Up;
        if (arma_actual == Hammer) {
            dir_arma = dir_mes_propera(p, dist_armes);
            prio = dist_armes[(p+dir_arma).i][(p+dir_arma).j];
        }
        else {
            Dir dir_arma = dir_mes_propera(p, dist_bazookas);
            prio = dist_bazookas[(p+dir_arma).i][(p+dir_arma).j];
        }
        return dir_arma;
    }

    void guerrers() {
        for (int id : meus_guerrers) {
            Citizen guerrer = citizen(id);
            WeaponType arma_actual = guerrer.weapon;
            Pos p = guerrer.pos;
            Dir direccio = Right;

            possibles_atacs(arma_actual);
            if (is_day()) {
                if (arma_actual == Hammer and (pos_pistoles.size() != 0 or pos_bazookas.size() != 0)) {
                    direccio = buscar_arma(p, arma_actual);
                }
                else if (arma_actual == Gun and pos_bazookas.size() != 0) {
                    direccio = buscar_arma(p, arma_actual);
                }
                else if (guerrer.life < warrior_ini_life()/2) {
                    direccio = dir_mes_propera(p, dist_diners_menjar);
                }
                else if (round()%25 > 20) {
                    direccio = dir_mes_propera(p, dist_enemics_guanyo);
                }
                else direccio = dir_mes_propera(p, dist_diners);
            }
            else {
                if (arma_actual == Bazooka or (arma_actual == Gun and pos_bazookas.size() == 0)) {
                    direccio = dir_mes_propera(p, dist_enemics_guanyo);
                    prio = dist_enemics_guanyo[(p+direccio).i][(p+direccio).j];
                }
                else if (arma_actual == Gun) {
                    direccio = dir_mes_propera(p, dist_enemics_guanyo_bazookas);
                    prio = dist_enemics_guanyo_bazookas[(p+direccio).i][(p+direccio).j];
                }
                else if (arma_actual == Hammer and pos_bazookas.size() != 0 and pos_pistoles.size() != 0) {
                    direccio = buscar_arma(p, arma_actual);
                }
                else {
                    direccio = dir_mes_propera(p, dist_enemics_guanyo);
                    prio = dist_enemics_guanyo[(p+direccio).i][(p+direccio).j];
                }
            }
            accions.push(Accio(id, direccio, prio));
        }
    }

    void constructors() {
        for (int id : meus_constructors) {
            Citizen constructor = citizen(id);
            Pos p = constructor.pos;
            Dir direccio;
            if (constructor.life < builder_ini_life()/2) {
                direccio = dir_mes_propera(p, dist_diners_menjar);
                prio = dist_diners_menjar[(p+direccio).i][(p+direccio).j];
            }
            else {
                direccio = dir_mes_propera(p, dist_diners);
                prio = dist_diners[(p+direccio).i][(p+direccio).j];
            }
            accions.push(Accio(id, direccio, prio));
        }
    }

    virtual void play () {
        reset();
        llegir_taulell();
        guerrers();
        constructors();
        fer_accions();
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);