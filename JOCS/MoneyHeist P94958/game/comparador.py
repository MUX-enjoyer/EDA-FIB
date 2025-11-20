'''	AQUEST SCRIPT EXECUTA n_partides I FA UNA COMPARATIVA ENTRE ELLES

	INPUT:
	python3 comparador.py <Nom1> <Nom2> <Nom3> <Nom4> <n_partides>

    OUTPUT:
    - n_Jugador: nº del jugador
    - NickJugador: Nom del jugador
    - Mitja_Punts: Mitjana de punts obtinguts
    - 1r: Nombre de cops que el jugador ha quedat en primera posició
    - 2n: Nombre de cops que el jugador ha quedat en segona posició
    - 3r: Nombre de cops que el jugador ha quedat en tercera posició
    - 4t: Nombre de cops que el jugador ha quedat en quarta posició
    - Pos_Mitja: Posició mitjana del jugador
    - %Victoria: Percentatge de passar a la següent fase (quedar 1r o 2n)

    RogeR Bit
'''

# Posar aquí ruta directori game
GAME_DIR = "."
# (on tingueu l'executable Game i els .o's)

import os
import random
import time
import subprocess
import math
from subprocess import Popen
from random import randint, shuffle
import re
import sys

def get_command(seed):
    c = "./Game "
    for i in range(0,4):
        c += PLAYERS[i] + " "
    c += "-s "
    c += str(seed)
    c += " -i default.cnf "
    c += "2>&1 > /dev/null | grep 'got score'"
    return c

if len(sys.argv) != 6:
    print("Error: s'han d'introduir exactament 5 arguments:\npython3 comparador.py Nom1 Nom2 Nom3 Nom4 n_partides")
    sys.exit(1)

try:
    os.chdir(GAME_DIR)
except OSError:
    print(f"Error: No s'ha pogut canviar al directori {GAME_DIR}")
    sys.exit(1)

random.seed(time.time())

PLAYERS = sys.argv[1:5]
punt = [0,0,0,0]
guany = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
n = int(sys.argv[5])
count = 0

try:
  for i in range(0,n):
    seed = random.randrange(10000)
    com = get_command(seed)
    p = Popen(com, shell=True, stdout=subprocess.PIPE)
    used = [0,0,0,0]
    llista = [0,0,0,0]
    for j,line in enumerate(p.stdout.readlines()):
        llista[j] += int(re.findall(r"\d+", str(line))[-1])

    maxim = -1
    jugador = 1
    for j in range(0,4):
      punt[j] += llista[j]
      if maxim < llista[j]:
          jugador = j
          maxim = llista[j]
    guany[0][jugador] += 1
    used[jugador] = 1
    print(str(i+1) + ". " + " "*(len(str(n))-len(str(i+1))) + "Ha guanyat el jugador " + str(jugador+1) + ": " + PLAYERS[jugador] + ". " + " "*(20 - len(str(PLAYERS[jugador]))) + "seed: " + str(seed))
    sys.stdout.flush()
    maxim = -1
    jugador = 1
    for j in range(0,4): 
      if maxim < llista[j] and used[j] == 0:
          jugador = j
          maxim = llista[j]
    guany[1][jugador] += 1
    used[jugador] = 1

    maxim = -1
    jugador = 1
    for j in range(0,4):
      if maxim < llista[j] and used[j] == 0:
          jugador = j
          maxim = llista[j]
    guany[2][jugador] += 1
    used[jugador] = 1

    maxim = -1
    jugador = 1
    for j in range(0,4):
      if maxim < llista[j] and used[j] == 0:
          jugador = j
          maxim = llista[j]
    guany[3][jugador] += 1
    used[jugador] = 1
    count = count+1 
  
except KeyboardInterrupt:
    print()

print ("")

nom = ["", "", "", ""]
mitja = [0.0,0.0,0.0,0.0]
clas = [0.0,0.0,0.0,0.0]
for i in range(0,4):
    nom[i] = sys.argv[i + 1]
    while len(nom[i]) < 12:
        nom[i] += str(" ")
    for j in range(0,4):
        mitja[i] += float(j + 1)*float(guany[j][i])/float(count)
    clas[i] = float(guany[0][i] + guany[1][i])/float(count)

print("Resultats de {} partides:\n".format(count))
print("{:<12} {:<13} {:<10} {:<3} {:<3} {:<3} {:<3} {:<10} {:<10}".format("", "Nom", "Mitja Pts", "1r", "2n", "3r", "4t", "Pos_Mitja", "%Victoria"))
for i in range(0, 4):
    print("{:<12} {:<13} {:<10} {:<3} {:<3} {:<3} {:<5} {:<8.2f} {:<10.2f}".format(
        "Jugador " + str(i + 1),
        nom[i].strip() + ":",
        punt[i] // count,
        guany[0][i],
        guany[1][i],
        guany[2][i],
        guany[3][i],
        mitja[i],
        clas[i]
    ))
