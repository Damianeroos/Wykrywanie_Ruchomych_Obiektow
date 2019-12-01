# WykrywanieObiektow

Celem projektu jest zaimplementowanie aplikacji okienkowej, która wczyta plik video zapisany na dysku lub obraz z kamery  i korzystając z algorytmów przetwarzania obrazów wykryje nowy/ruchomy obiekt i oznaczy go. Przetworzony plik video zostanie zapisany, a następnie wyświetlony w oknie aplikacji. Obraz modyfikowany jest w czasie rzeczywistym. Nową funkcjonalnością jest ręcznie ustawiana wirtualna bramka, która zlicza przejazdy wykrytych obiektów. Do wykrywania ruchomego obiektu na scenie wybrano metodę "background subtraction" MOg2.

Narzędzia:
   - system operacyjny: Ubuntu 18.04
   - edytor tekstu: QtCreator
   - język programowania, kompilator: C++, g++
   - dokumentacja html: Doxygen
   - aplikacja okienkowa: biblioteki Qt
   -  przetwarzanie  pliku video:  biblioteka OpenCV 
   - system kotroli wersji git: gitHub


Opis katalogów :
./testy - zawiera opis przeprowadzonych testów manualnych
./src   - kody źródłowe klas
./inc   - pliki nagółówkowe klas
./doc   - folder z dokumentacją html

Bibliografia:
https://sound.eti.pg.gda.pl/student/pdio/pdio05_detekcja_sledzenie_obiektow.pdf

```diff
- Do poprawnego działania aplikacji należy zainstalować bibliotekę opencv
```
https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
