# WykrywanieObiektow

Celem projektu jest zaimplementowanie aplikacji okienkowej, która wczyta plik video zapisany na dysku lub obraz z kamery  i korzystając z algorytmów przetwarzania obrazów wykryje nowy/ruchomy obiekt i oznaczy go. Przetworzony plik video zostanie zapisany, a następnie wyświetlony w oknie aplikacji. W zależności od postępu prac nad aplikacją planowane jest dodanie nowych funkcjonalności, takich jak wyświetlanie przetworzonych sekwencji obrazów w czasie rzeczywistym, śledzenie wykrytego obiektu lub rozpoznawanie obiektu (za pomocą gotowych sieci neuronowych). Do wykrywania ruchomego obiektu na scenie wybrano metodę "background subtraction".

Narzędzia:
   - system operacyjny: Ubuntu 18.04
   - edytor tekstu: emacs
   - język programowania, kompilator: C++, g++
   - dokumentacja html: Doxygen
   - aplikacja okienkowa: biblioteki Qt
   -  przetwarzanie  pliku video:  biblioteka OpenCV + OpenCL (umożliwia wykorzystanie gpu)
   - system kotroli wersji git: gitHub


Bibliografia:
https://sound.eti.pg.gda.pl/student/pdio/pdio05_detekcja_sledzenie_obiektow.pdf