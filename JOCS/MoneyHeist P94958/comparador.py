'''
    AQUEST SCRIPT COMPARA EL RENDIMENT DE 4 JUGADORS (AIs)
    EXECUTANT MÚLTIPLES PARTIDES I CALCULANT ESTADÍSTIQUES.

    PREREQUISITS:
    - Executable './Game' al mateix directori
    - Fitxer 'default.cnf' al mateix directori
    - Els 4 jugadors (programes AI) especificats

    INPUT:
    python3 comparador.py <jugador1> <jugador2> <jugador3> <jugador4> <num_partides>

    OUTPUT: 
    - Jugador X: Identificador del jugador
    - Punts Mitjos: Puntuació mitjana per partida
    - 1r, 2n, 3r, 4t: Nombre de vegades que ha quedat en cada posició
    - Pos Mitja: Posició mitjana ponderada (1.00 = sempre 1r, 4.00 = sempre 4t)
    - % Clas: Percentatge de classificació al primer torn (1r + 2n)
    
    NOTES:
    - Les partides s'executen amb seeds aleatòries
    - Es mostra el guanyador de cada partida en temps real
    - Durant l'execució es pot aturar amb Ctrl+C i mostrarà
    les estadístiques parcials fins aquell moment.
'''

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
      llista[j] += int(re.findall("\d+", str(line))[-1])

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

for i in range(0,4):
  print("Jugador "+str(i + 1) + " " + nom[i] + ": " + str(punt[i]//count) + " " + str(guany[0][i]) + " "\
+ str(guany[1][i]) + " " + str(guany[2][i]) + " " + str(guany[3][i]) + "  %.2f" % (mitja[i]) + "  %.2f" % (clas[i]))
