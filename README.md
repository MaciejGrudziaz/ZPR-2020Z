# Projekt ZPR 2020Z

#### Inicjalizacja projektu
Po sklonowaniu repo należy wykonać następujące kroki:
1. Z poziomu głównego katalogu projektu `./` pobierz wszystkie submoduły
   ```
   git submodule --init --recursive
   ```
   Ten proces może potrwać kilkanaście minut
2. Zbuduj projekt testowy
   ```
   mkdir build
   cd build
   cmake ..
   make 
   ```
3. Uruchom przykładowe testy
   ```
   cd build/atomic-list/test
   ./ZPR-2020Z-atomic-list-test
   ```
   Jeśli wszystko poszło pomyślnie program powinien zakończyś się jednym poprawnie wykonanym testem