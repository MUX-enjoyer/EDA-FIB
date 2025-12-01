''' 
    FET PER: RogeR Bitlloch

    AQUEST SCRIPT VA A LA COMPETICIÓ DEL JUTGE I CALCULA
    LA MITJANA DE PUNTUACIÓ ENTRE LES RONDES QUE LI DIEU
    I LA PROBABILITAT D'HAVER SUPERAT EL PRIMER TORN D'AQUELLA RONDA.

    INPUT:
    python3 calc_mitja.py <min_round> <max_round>
    
    recomano redirigir l'OUTPUT a un .txt escrivint:
    python3 calc_mitja.py <min_round> <max_round> > mitja_jocEDA.txt

    OUTPUT: 
    - Posició
    - Nick del codi
    - Score = Pts Mitja*log(T1 + 1)
    - Puntuació mitja
    - % de Victoria (haver quedat 1r o 2n al 1r torn)
    - Partides totals jugades
    ---------------
    - %Victoria Torn X
    - Partides jugades Torn X
'''

# Posar aquí la URL del joc a consultar (pestanya rounds)
BASE_URL = "https://jutge.org/competitions/EDA:EDA_Q1_2025_26/rounds"
# ANIREM AFEGINT EL NUMERO DE RONDA DARRERA LA URL PER ANAR CONSULTANT LA PUNTUACIÓ

import sys
import requests
from bs4 import BeautifulSoup
import math

def fetch_and_parse_html(round_number):
    url = f"{BASE_URL}/{round_number}"
    response = requests.get(url)
    soup = BeautifulSoup(response.text, 'html.parser')
    return soup

def extract_turn_table(soup, turn):
    """ Retorna la taula de cada torn a partir del número del torn """
    turn_heading = soup.find('b', string=f'Turn {turn}')
    if turn_heading:
        table = turn_heading.find_next('table', {'class': 'table table-striped'})
        return table
    return None

def extract_player_data(soup):
    """ Extreu les dades dels jugadors d'un objecte BeautifulSoup per tots els torns """
    players_data = {}
    for turn in range(1, 6):
        table = extract_turn_table(soup, turn)
        if table:
            rows = table.find_all('tr')
            for row in rows:
                cells = row.find_all('td')
                if len(cells) > 1:  # Asumim que hi ha més d'una cel·la (nom i puntuació)
                    for i in range(1, len(cells), 2):  # Saltem cada dues cel·les per obtenir parells de nom-puntuació
                        player_name = cells[i].get_text(strip=True)
                        score = int(cells[i + 1].get_text(strip=True))
                        success_class = cells[i]['class']
                        passed_round = 'text-danger' not in success_class

                        if turn == 1 or (turn > 1 and player_name in players_data and not players_data[player_name]['passed_previous'][turn - 1]):
                            if player_name in players_data:
                                players_data[player_name]['total_score'] += score
                                players_data[player_name]['rounds'][turn] += 1
                                if passed_round:
                                    players_data[player_name]['rounds_passed'][turn] += 1
                                players_data[player_name]['passed_previous'][turn] = passed_round
                            else:
                                players_data[player_name] = {
                                    'total_score': score,
                                    'rounds': {1: 0, 2: 0, 3: 0, 4: 0, 5: 0},
                                    'rounds_passed': {1: 0, 2: 0, 3: 0, 4: 0, 5: 0},
                                    'passed_previous': {1: False, 2: False, 3: False, 4: False, 5: False}
                                }
                                players_data[player_name]['rounds'][turn] = 1
                                if passed_round:
                                    players_data[player_name]['rounds_passed'][turn] = 1
                                players_data[player_name]['passed_previous'][turn] = passed_round
    return players_data

def main(min_round, max_round):
    all_players = {}
    for round_number in range(min_round, max_round + 1):
        soup = fetch_and_parse_html(round_number)
        players = extract_player_data(soup)
        for player, data in players.items():
            if player in all_players:
                all_players[player]['total_score'] += data['total_score']
                for t in range(1, 6):
                    all_players[player]['rounds'][t] += data['rounds'][t]
                    all_players[player]['rounds_passed'][t] += data['rounds_passed'][t]
                all_players[player]['passed_previous'] = data['passed_previous']
            else:
                all_players[player] = data

    # Calcula la puntuació ponderada
    for player, data in all_players.items():
        total_rounds = sum(data['rounds'].values())
        if total_rounds > 0:
            average_score = data['total_score'] / total_rounds
            pass_probability = sum(data['rounds_passed'].values()) / sum(data['rounds'].values())
            weighted_score = average_score * math.log(data['rounds'][1] + 1) * pass_probability
            data['weighted_score'] = weighted_score
        else:
            data['weighted_score'] = 0

    # Ordena els jugadors per la puntuació ponderada descendent
    sorted_players = sorted(all_players.items(), key=lambda x: x[1]['weighted_score'], reverse=True)

    # Mostra les dades en una taula ben formatada
    print("Resultats entre rondes ", str(min_round), " i ", str(max_round),"\n")
    print("Score = Mitja Pts * log(Rondes + 1) * %Victoria")
    print("És la millor manera que he trobat de comparar:\npunts, nº de rondes i WinRate%\n\n")
    print("{:<5} {:<15} {:<10} {:<10} {:<10} {:<6} || {:<7} {:<3} | {:<7} {:<3} | {:<7} {:<3} | {:<7} {:<3} | {:<7} {:<3}".format(
        "Pos", "Nick del codi", "Score", "Mitja Pts", "%Victoria", "Games", "%T1", "T1", "%T2", "T2", "%T3", "T3", "%T4", "T4", "%T5", "T5"
    ))
    i = 0
    for player, data in sorted_players:
        i += 1
        average_score = data['total_score'] / sum(data['rounds'].values())
        pass_probability = sum(data['rounds_passed'].values()) / sum(data['rounds'].values())
        total_rounds = sum(data['rounds'].values())
        t1_jugats = data['rounds'][1]
        t1_victory = data['rounds_passed'][1] / data['rounds'][1] if data['rounds'][1] > 0 else 0
        t2_jugats = data['rounds'][2]
        t2_victory = data['rounds_passed'][2] / data['rounds'][2] if data['rounds'][2] > 0 else 0
        t3_jugats = data['rounds'][3]
        t3_victory = data['rounds_passed'][3] / data['rounds'][3] if data['rounds'][3] > 0 else 0
        t4_jugats = data['rounds'][4]
        t4_victory = data['rounds_passed'][4] / data['rounds'][4] if data['rounds'][4] > 0 else 0
        t5_jugats = data['rounds'][5]
        t5_victory = data['rounds_passed'][5] / data['rounds'][5] if data['rounds'][5] > 0 else 0

        print("{:<5} {:<15} {:<10.2f} {:<10.2f} {:<10.2%} {:<6} || {:<7.2%} {:<3} | {:<7.2%} {:<3} | {:<7.2%} {:<3} | {:<7.2%} {:<3} | {:<7.2%} {:<3}".format(
            i, player, data['weighted_score'], average_score, pass_probability, total_rounds, t1_victory, t1_jugats, t2_victory, t2_jugats, t3_victory, t3_jugats, t4_victory, t4_jugats, t5_victory, t5_jugats
        ))

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 calc_mitja.py <min_round> <max_round>")
        sys.exit(1)
    min_round = int(sys.argv[1])
    max_round = int(sys.argv[2])
    main(min_round, max_round)
