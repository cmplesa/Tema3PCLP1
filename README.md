# Tema3PCLP1

TEMA 3:

Am folosit 2 structuri pentru retinerea datelor. Structura image contine datele
imaginii si pentru matricea color mai folosesc o structura care retine valoarea
pentru pixelii red, green sau blue.

LOAD:

Aceasta functie se realizeaza prin 3 functii (load, loadgray, loadcolor).
De fiecare data verific daca o alta imagine este incarcata, daca este eliberez
memoria pentru alta noua. La functia load se verifica mai intai tipul imaginii
colora/gri si dupa este apelata una dintre cele 2 functii, care au aproximativ
aceeasi structura. Deschid fisierul pentru a citi datele despre imagine, dupa
setez anumite valori ale imaginii si aloc o matrice colora/gri, depinde de
tipul imaginii pe care il aflam deja din functia load. Fisierul e deschis
initial text, dar daca este binar, se inchide si se deschide in mod binar. In
aceste functii mai gasim si metoda pentru eliminarea comentariilor din 
fisierele incarcate.

SAVE:

Aceasta functie se realizeaza prin functia save care primeste ca parametru o
imagine si comanda pentru a verifica daca salvarea se cere a fi facuta in mod
binar sau ascii, am facut acest lucru prin sscanf si am verificat prin
variabila rezultat. Dupa in functie de variabila ok, se deschide fisierul in
mod text sau binar si se realizeaza salvarea fiecarei tip de imagine,

SELECT:

Functia select se realizeaza prin 2 functii selectioncoordinates si selectall.
In functia select all, sunt atribuite vectorului de coordonate, valorile
matricei. In ambele functii, se verifica mai intai daca este incarcata o
imagine. In functia selectioncoordinates am realizat si interschimbarea 
printr-o functie de interschimbare definita mai sus si verifica regulile de 
parametrii. In main se verifica daca c1 este SELECT si dupa prin variabila 
rezultat vedem daca s-a introdus doar SELECT sau SELECT ALL.

EXIT:

Functia se realizeaza prin functia gracefulexit si elibereaza memoria folosita.

HISTOGRAM:

Se realizeaza histograma imaginii cu y binuri si x stelute prin functia
histogram. Prima oara se verifica acele conditii cerute de cerinta si dupa
functia construieste histograma imaginii in acel vector de frecventa si dupa
este apelata functia equalizer care ajusteazafrecvența valorilor și 
a realiza egalizarea histogramelor. Numarul de stelute este calculat cu formula
data.

CROP:

Se realizeaza prin functia crop si mai intai se verifica daca este incarcata
o imagine. Sunt calculate noile valori ale matricei auxiliare, in functie de
culoarea imaginii acestea se aloca prin 2 functii diferite, se copiaza
elementele din cea originala in cea temporara, dupa este eliberata memoria
matricei originale si este din nou alocata cu noile valori calculate la
inceput. Dupa acest pas sunt copiate valorile din auxiliara in matricea
redimensionata si se elibereaza memoria matricei auxiliare.

EQUALIZE:

Aceasta functie se realizeaza prin functia equalize si construiește 
histograma imaginii pentru intensitățile de gri folosind un vecot
numit histogram. Dupa construiește histograma cumulativă 
(cumulativehistogram) pe baza histogramelor individuale. Se realizeaza
calculul pentru fiecare nou pixels si prin functia de clamp, i se aduce
valoarea intr-un interval acceptat.

APPLY:

Prima oara se verifica prin functia apply: parametrii comenzii, tipul de
imagine si daca o imagine este incarcata sau nu. Daca imaginea incarcata
este color, atunci se apeleaza functia applyeffect unde se initializeaza
o matrice auxiliara rgb si sunt copiate elementele din matricea originala
in cea auxiliara. Se aloca dinamic matricea pentru filtru si este 
initializata prin functia initializekernel care decide pe baza stringului
filter cu ce valori este initiata matricea. Se realizeaza inmultirea
pixelilor cu matricea kernel si se copiaza invers dupa elementele si dupa
se da free la matricea kernel si cea auxiliara.
