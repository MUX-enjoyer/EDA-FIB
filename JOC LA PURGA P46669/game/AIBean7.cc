#include "Player.hh"
#include <limits>
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Bean7


struct PLAYER_NAME : public Player {

    static Player* factory () {
        return new PLAYER_NAME;
    }

    //      CONSTANTS      //

    using VInt  = vector<int>;
    using VCela = vector<Cell>;
    using VDir  = vector<Dir>;
    using VPos  = vector<Pos>;
    using VIntPos = vector<pair<int, Pos>>;

    using MInt  = vector<VInt>;
    using MCela = vector<VCela>;
    using MIntPos = vector<VIntPos>;

    const Dir NoneDir = Dir(-1);
    const VDir dirs = {Up, Down, Left, Right, NoneDir};
    const VInt x = {0, 0, -1, 1};
    const VInt y = {-1, 1, 0, 0};


    const int MAX = numeric_limits<int>::max();
        
    struct Accio {
        int id;
        Dir dir;
        pair<int, Pos> objectiu;

        bool operator>(const Accio& a) const {
            return objectiu.first > a.objectiu.first;
        }

        Accio() : id(-1), dir(Up), objectiu({0,{-1, -1}}) {}
        Accio(int id, Dir dir, pair<int, Pos> obj) : id(id), dir(dir), objectiu(obj) {}
    };

    //      VARIABLES      //

    VPos pos_bazookas, pos_pistoles, pos_guerrers_enemics, pos_constructors_enemics, pos_diners, pos_menjar;
    
    VInt meus_guerrers;
    VInt meus_constructors;

    MIntPos dist_bazookas;
    MIntPos dist_armes;
    MIntPos dist_guerrers_enemics;
    MIntPos dist_enemics_guanyo;
    MIntPos dist_enemics_guanyo_bazookas;
    MIntPos dist_diners;
    MIntPos dist_diners_menjar;

    priority_queue<Accio, vector<Accio>, greater<Accio>> accions;
    pair<int, Pos> objectiu;
    set<Pos> objectius_agafats;
    int recalculs;

    //      CONSULTORES      //

    bool es_carrer(const Pos& p) {
        return cell(p).type == Street;
    }
    bool es_enemic(const Pos& p) {
        return cell(p).id != -1 and citizen(cell(p).id).player != me();
    }
    bool es_enemic_guerrer(const Pos& p) {
        return es_enemic(p) and citizen(cell(p).id).type == Warrior;
    }
    bool es_barricada_enemiga(const Pos& p) {
        return cell(p).b_owner != -1 and cell(p).b_owner != me();
    }
    bool cela_lliure(const Pos& p) {
        return pos_ok(p) and es_carrer(p) and cell(p).id == -1 and !es_barricada_enemiga(p);
    }
    bool es_arma(const Pos& p) {
        return cell(p).weapon != NoWeapon;
    }

    /*  Omple mapa amb les distàncies relatives a les
        pos del vector pos_origen (per saber el path) */
    void buscar_mapa(VPos posicions_origen, MIntPos& mapa, int multi) {
        for (Pos pos_origen : posicions_origen) {
            queue<Pos> caselles_pendents;
            if (pos_ok(pos_origen) and !es_barricada_enemiga(pos_origen)) {
                caselles_pendents.push(pos_origen);
                mapa[pos_origen.i][pos_origen.j] = {0, pos_origen};
            }

            while(!caselles_pendents.empty()) {
                Pos pos_cela = caselles_pendents.front();
                caselles_pendents.pop();

                for (Dir d : dirs) {
                    Pos new_pos_cela = pos_cela + d;
                    int new_dist = mapa[pos_cela.i][pos_cela.j].first + multi;
                    if (cela_lliure(new_pos_cela) and mapa[new_pos_cela.i][new_pos_cela.j].first > new_dist) {
                        caselles_pendents.push(new_pos_cela);
                        mapa[new_pos_cela.i][new_pos_cela.j] = {new_dist, pos_origen};
                    }
                }
            }
        }
    }

    /*  Retorna la direccio que mes s'apropa
        a l'objecte del mapa */
    Dir dir_mes_propera(const Pos& p, const MIntPos& mapa) {
        Dir dir_propera = NoneDir;
        int min_dist = MAX;
        for (Dir d : dirs) {
            Pos next_p = p + d;
            if (pos_ok(next_p) and min_dist > mapa[next_p.i][next_p.j].first) {
                dir_propera = d;
                min_dist = mapa[next_p.i][next_p.j].first;
            }
        }
        return dir_propera;
    }

    /* Fa el contrari a Dir_mes_propera,
    Retorna la direccio que més s'alluna a l'objecte del mapa */
    Dir fugir(const Pos& p, const MIntPos& mapa) {
        Dir dir_llunyana = NoneDir;
        int max_dist = 0;
        for (Dir d : dirs) {
            Pos next_p = p + d;
            if (cela_lliure(next_p) and max_dist < mapa[next_p.i][next_p.j].first) {
                dir_llunyana = d;
                max_dist = mapa[next_p.i][next_p.j].first;
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
        objectius_agafats.clear();

        dist_bazookas = dist_armes = dist_guerrers_enemics = dist_enemics_guanyo = dist_enemics_guanyo_bazookas =
        dist_diners = dist_diners_menjar = MIntPos(board_rows(), VIntPos(board_cols(), {MAX, {-1 ,-1}}));

        objectiu = {MAX, {-1, -1}};
        recalculs = 0;
    }

    void llegir_taulell() {
        for (int i = 0; i < board_rows(); ++i) {
            for (int j = 0; j < board_cols(); ++j) {
                Pos p = Pos({i, j});
                Cell casella = cell(p);
                if (casella.weapon == Bazooka) pos_bazookas.push_back(p);
                else if (casella.weapon == Gun) pos_pistoles.push_back(p);
                else if (es_enemic_guerrer(p)) pos_guerrers_enemics.push_back(p);
                else if (es_enemic(p)) pos_constructors_enemics.push_back(p);
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

    void possibles_atacs(WeaponType arma, int vida) {
        VPos pos_enemics_guanyo = pos_constructors_enemics;
        for (Pos p : pos_guerrers_enemics) {
            Citizen enemic = citizen(cell(p).id);
            WeaponType arma_enemic = enemic.weapon;
            if (arma_enemic != strongestWeapon(arma, arma_enemic)) {
                pos_enemics_guanyo.push_back(p);
            }
        }
        buscar_mapa(pos_enemics_guanyo, dist_enemics_guanyo, 1);
        buscar_mapa(pos_enemics_guanyo, dist_enemics_guanyo_bazookas, 3);
    }

    Dir buscar_arma(const Pos& p, const WeaponType& arma_actual) {
        Dir dir_arma = dir_mes_propera(p, arma_actual == Hammer ? dist_armes : dist_bazookas);
        objectiu = (arma_actual == Hammer ? dist_armes : dist_bazookas)[(p+dir_arma).i][(p+dir_arma).j];
        return dir_arma;
    }

    void guerrers(int id) {
        Citizen guerrer = citizen(id);
        WeaponType arma_actual = guerrer.weapon;
        Pos p = guerrer.pos;
        Dir direccio = NoneDir;

        possibles_atacs(arma_actual, guerrer.life);
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
            else if (round()%25 > 15 and dist_enemics_guanyo[p.i][p.j].first > 25-round()%25) {
                direccio = dir_mes_propera(p, dist_enemics_guanyo);
            }
            else direccio = dir_mes_propera(p, dist_diners);
        }
        else {
            if (arma_actual == Bazooka or (arma_actual == Gun and pos_bazookas.size() == 0)) {
                direccio = dir_mes_propera(p, dist_enemics_guanyo);
                objectiu = dist_enemics_guanyo[(p+direccio).i][(p+direccio).j];
            }
            else if (arma_actual == Gun) {
                direccio = dir_mes_propera(p, dist_enemics_guanyo_bazookas);
                objectiu = dist_enemics_guanyo_bazookas[(p+direccio).i][(p+direccio).j];
            }
            else if (arma_actual == Hammer and pos_bazookas.size() != 0 and pos_pistoles.size() != 0) {
                direccio = buscar_arma(p, arma_actual);
            }
            else {
                direccio = dir_mes_propera(p, dist_enemics_guanyo);
                objectiu = dist_enemics_guanyo[(p+direccio).i][(p+direccio).j];
            }
        }
        accions.push(Accio(id, direccio, objectiu));
    }

    void constructors(int id) {
        Citizen constructor = citizen(id);
        Pos p = constructor.pos;
        Dir direccio = NoneDir;
      //  if (!is_day() and dist_guerrers_enemics[p.i][p.j].first < 4) {
      //      direccio = fugir(p, dist_guerrers_enemics);
      //      objectiu = dist_guerrers_enemics[(p+direccio).i][(p+direccio).j];
      //  }
        if (constructor.life < builder_ini_life()/2) {
            direccio = dir_mes_propera(p, dist_diners_menjar);
            objectiu = dist_diners_menjar[(p+direccio).i][(p+direccio).j];
        }
        else {
            direccio = dir_mes_propera(p, dist_diners);
            objectiu = dist_diners[(p+direccio).i][(p+direccio).j];
        }
        accions.push(Accio(id, direccio, objectiu));
    }

    void treure_pos(Pos p_obj, VPos& v_pos) {
        VPos v_aux;
        for (Pos p : v_pos) {
            if (p_obj != p) v_aux.push_back(p);
        }
        v_pos = v_aux;
    }

    void recalcular(Pos p) {
        treure_pos(p, pos_bazookas);
        treure_pos(p, pos_pistoles);
        treure_pos(p, pos_diners);
        treure_pos(p, pos_menjar);

        dist_bazookas = dist_armes = dist_enemics_guanyo_bazookas =
        dist_diners = dist_diners_menjar = MIntPos(board_rows(), VIntPos(board_cols(), {MAX, {-1 ,-1}}));

        buscar_mapa(pos_bazookas, dist_bazookas, 1);
        buscar_mapa(pos_bazookas, dist_armes, 1);
        buscar_mapa(pos_pistoles, dist_armes, 2);
        buscar_mapa(pos_diners, dist_diners, 1);
        buscar_mapa(pos_menjar, dist_diners_menjar, 1);
    }

    bool enemic_aprop_bazooka(int id, const Pos pos_obj) {
        if (cell(pos_obj).weapon == Bazooka) {
            Pos p = citizen(id).pos;
            int dist = dist_bazookas[p.i][p.j].first;

            for (Pos pos_ene : pos_guerrers_enemics) {    
                pair<int, Pos> info_ene = dist_armes[pos_ene.i][pos_ene.j];
                WeaponType arma_ene = citizen(cell(pos_ene).id).weapon;
                if (pos_obj == info_ene.second and dist > info_ene.first and arma_ene != Bazooka) {
                    return true;
                }
            }
        }
        return false;
    }

    void fer_accions() {
        while(!accions.empty()) {
            Accio a = accions.top();
            accions.pop();
            
            int id = a.id;
            Pos p = a.objectiu.second;
            Dir d = a.dir;            
            if ((objectius_agafats.find(p) == objectius_agafats.end() and !enemic_aprop_bazooka(id, p)) or es_enemic(p)) {
                if (d != NoneDir) move(id, d);
                objectius_agafats.insert(p);
                recalculs = 0;
            }
            else {
                if (++recalculs < 5) {
                    recalcular(p);
                    citizen(id).type == Warrior ? guerrers(id) : constructors(id);
                } 
                else if (d != NoneDir) move(id, d);
            }
        }
    }

    virtual void play () {
        reset();
        llegir_taulell();
        for (int id : meus_guerrers) guerrers(id);
        for (int id : meus_constructors) constructors(id);
        fer_accions();
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
