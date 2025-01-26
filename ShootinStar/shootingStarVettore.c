/****************************************************************************
 *
 * shooting-stars.c - Shooting Stars
 *
 * Copyright (C) 2018--2024 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

/***
% LabASD - Shooting Stars
% Moreno Marzolla <moreno.marzolla@unibo.it>
% Ultimo aggiornamento: 2024-02-26

![](shooting-stars.jpg "Copertina del numero 9, maggio 1976 di BYTE Magazine dedicata a shooting stars")

"Shooting stars" è un gioco di strategia descritto nella rivista [BYTE
Magazine n. 9, p. 42, maggio
1976](https://archive.org/details/byte-magazine-1976-05/page/n43/mode/1up).

Il gioco si svolge su una griglia $3 \times 3$ le cui celle sono
numerate da 0 a 8 come segue:

```
012
345
678
```

Ogni cella può contenere una stella (`*`) oppure un buco nero
(`.`). La configurazione iniziale contiene un'unica stella nella cella
4:

```
...
.*.
...
```

Ad ogni turno il giocatore può far "esplodere" una stella digitando il
numero della cella corrispondente. Una stella esplosa si trasforma in
un buco nero; inoltre, i frammenti della stella vanno a finire in
alcune delle celle adiacenti, trasformando i buchi neri in stelle, e
le stelle in buchi neri. Il vicinato è definito in modo differente per
ogni cella, ed è rappresentato con un `#` dalle figure seguenti:

```
0#.  #1#  .#2
##.  ...  .##
...  ...  ...

#..  .#.  ..#
3..  #4#  ..5
#..  .#.  ..#

...  ...  ...
##.  ...  .##
6#.  #7#  .#8
```

quindi il vicinato della cella 0 è costituita da (1, 3, 4); il
vicinato della cella 1 è costituito da (0, 2); il vicinato della cella
2 è costituito da (1, 4, 5), e così via. In pratica, fare esplodere la
stella in posizione $k$ significa trasformare stelle in buchi neri (e
viceversa) nella cella $k$ e in quelle "vicine" secondo le figure
sopra.

Ad esempio, partendo dalla configurazione iniziale (per facilitare la
lettura, le celle contenenti una stella sono indicate con il
rispettivo numero):

```
...
.4.
...
```

il giocatore può solo fare esplodere la stella 4, ottenendo la nuova
configurazione

```
.1.
3.5
.7.
```

Se ora decide di fare esplodere la stella 3, la nuova configurazione
sarà

```
01.
..5
67.
```

e così via.

Si vince se si ottiene la configurazione

```
012
3.5
678
```

mentre si perde se si ottiene la configurazione contenente solo buchi neri

```
...
...
...
```

(infatti, a questo punto non sarebbe possibile far esplodere alcuna
stella).

Compilare il programma con

        gcc -std=c90 -Wall -Wpedantic shooting-stars.c -o shooting-stars

In ambiente Linux/MacOSX eseguire con

        ./shooting-stars

In ambiente Windows eseguire con

        .\shooting-stars

## Per approfondire

Alla fine del corso sarete in grado di scrivere un programma
efficiente in grado di rispondere a queste domande:

- Qual è il numero minimo di mosse necessarie per vincere? Qual è la
  sequenza minima di mosse che porta alla vittoria?

- Qual è il numero minimo di mosse necessarie a perdere (cioè
  a raggiungere la configurazione vuota)?

- Esiste una configurazione di stelle/buchi neri che non può essere
  generata dallo stato iniziale effettuando solo mosse valide?

Per il momento si richiede di completare il programma in modo da
consentire all'utente di giocare una partita.

## File

- [shooting-stars.c](shooting-stars.c)

***/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void hide(unsigned int table[],unsigned int attiva[]){
  int i;
  for(i=0;i<=8;i++){
    if(i%3==0) printf("\n");
    if(attiva[i]){
      printf("%d",table[i]);
      /*printf("*");*/
    }else{
      printf(".");
    }
  }
}

unsigned int isvalid(int move,unsigned int attiva[]){
  if(attiva[move]){
    return 1;
  }
  return 0;
}


int main( void )
{ 
    int c,k=0;

    int move;
    unsigned int table[] = {0,1,2,3,4,5,6,7,8};
    unsigned int attiva[] = {0,0,0,0,1,0,0,0,0};
    hide(table,attiva);
    scanf("\n%d",&move);
  while(move>=0){
    if(isvalid(move,attiva)){
      switch (move){
        case 0:
          attiva[0]=!attiva[0];
          attiva[1]=!attiva[1];
          attiva[3]=!attiva[3];
          break;
        case 1:
        case 7:
          attiva[move-1]=!attiva[move-1];
          attiva[move]=!attiva[move];
          attiva[move+1]=!attiva[move+1];
          break;
        case 2:
          attiva[1]=!attiva[1];
          attiva[2]=!attiva[2];
          attiva[5]=!attiva[5];
          break;
        case 3:
        case 5:
          attiva[move-3]=!attiva[move-3];
          attiva[move]=!attiva[move];
          attiva[move+3]=!attiva[move+3];
          break;
        case 4:
          attiva[1]=!attiva[1];
          attiva[3]=!attiva[3];
          attiva[4]=!attiva[4];
          attiva[5]=!attiva[5];
          attiva[7]=!attiva[7];
          break;
        case 6:
          attiva[3]=!attiva[3];
          attiva[6]=!attiva[6];
          attiva[7]=!attiva[7];
          break;
        case 8:
          attiva[5]=!attiva[5]; 
          attiva[7]=!attiva[7];
          attiva[8]=!attiva[8];
          break;
      }
    }
    hide(table,attiva);
    for(c=0;c<=8;c++){
      if(attiva[c]==1){
        k++;
      }
    }
    if(k==0){
      printf("Hai perso!");
    }else if(k==8){
      printf("Hai vinto!");
      return EXIT_SUCCESS;
    }
    scanf("\n%d",&move);
  }
    return EXIT_SUCCESS;
    
}


