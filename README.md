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
   cmake ../
   make 
   ```
3. Uruchom przykładowy program i testy
   ```
   cd build
   ./ZPR-2020Z
   ./test/ZPR-2020Z-test
   ```