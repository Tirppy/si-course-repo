# Lab 1.1 Interacțiunea cu Utilizatorul: STDIO - Interfața Serială

## Scopul lucrării

Familiarizarea studenților cu utilizarea bibliotecii **STDIO** pentru **comunicare serială** și implementarea unei aplicații simple care controlează un **LED** prin comenzi text transmise de la un **terminal serial**.

## Obiectivele lucrării

- Înțelegerea principiilor de bază ale comunicației seriale.
- Utilizarea bibliotecii **STDIO** pentru schimbul de informații text.
- Proiectarea unei aplicații care interpretează comenzi transmise prin interfața serială.
- Dezvoltarea unei soluții **modulare**, cu funcționalități separate pentru controlul perifericelor.

## Definire problema

1. Sa se configureze aplicatia pentru lucrul cu libraia STDIO prin interfata seriala pentru schimbul de text prin terminal.
2. Sa se proiecteze o aplicatie in baza de MCU care ar receptiona comenzi de la terminal prin interfata seriala pentru a seta starea unui LED.
   - ***led on*** pentru aprindere si
   - ***led off*** pentru stingere.
   - sistemul trebuie sa raspunda cu mesaje text despre confirmarea comenzii
   - pentru schimbul de text prin terminal a se utiliza libraia STDIO

## Materiale și resurse

### Componente hardware:

- **Microcontroler** (Arduino Uno, ESP32 sau similar)
- **LED-uri**
- **Rezistor de 220 Ω**
- **Breadboard**
- **Cabluri de conexiune (jumper wires)**
- **Sursă de alimentare (USB)**

### Resurse software:

- **Visual Studio Code** cu extensia **PlatformIO** instalată
- **Emulator terminal serial** (ex.: **Monitor Serial** din PlatformIO, **TeraTerm** sau **Putty**)
- **Simulator hardware** *(opțional, ex.: Proteus)*

## Recomandari

- Se recomanda utilizarea unui IDE cu suport arduino, care permite lucrul cu mai multe fisiere. De exemplu Eclipse.
- Pentru validare se recomanda utilizarea unui simulator, de ex Proteus.
- Functionalitatile pentru fiecare echipament periferic (led, buton, lcd, keypad) sa fie realizate in fisiere separate, in scop de reutilizare in lucrarile urmatoare.
- Utilizarea regulilor de codare ***CamelCase***.

## ___Pontaj___

### Cerințe obligatorii:

- Respectarea structurii modulare a proiectului.
- Prezentarea **schițelor arhitecturale, schemelor bloc și schemelor electrice**.
- Respectarea formatului raportului conform normelor UTM.

### Notare

- **Nu se accepta rapoartele care nu corespund cerintelor de prezentare**
- **50%** demonstrarea rezultatelor.
- **10%** utilizarea **STDIO** (`printf`, `scanf`, etc.).
- **10%** explicarea interfeței hardware-software stackuril de periferii.
- **10%** prezentarea **schiței electrice**.
- **10%** demonstrarea funcționării fizice.

**NOTĂ:** **Pontajul maxim este posibil doar la prezentarea funcționării fizice a soluției!!**

- **-10%** penalizare pentru fiecare săptămână întârziere de la deadline.
