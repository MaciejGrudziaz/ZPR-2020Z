# Projekt ZPR 2020Z

#### Inicjalizacja projektu
Po sklonowaniu repozytorium należy wykonać następujące kroki:
1. Pobranie submodułów (`boost` oraz `googletest`)
   ```
   git submodule update --init --recursive
   ```
   Ten proces może potrwać kilkanaście minut
2. Zbudowanie testów (`atomic/test`)
   ```
   mkdir build
   cd build
   cmake ..
   make 
   ```
3. Uruchomienie testów
   ```
   cd build/atomic/test
   ./ZPR-2020Z-atomic-list-test
   ```

#### Uruchomienie testów w Dockerze
Projekt można również przetestować przy użyciu `Docker`'a. W tym celu należy z 
poziomu katalogu `./atomic/` zbudować obraz Docker'a:
```bash
docker build -t zpr-test .
```
nazwa **zpr-test** jest ważna jeśli będziemy chcieli uruchomić później program 
sprawdzający pokrycie kodu testami.

Testy uruchamiamy poleceniem
```bash
docker run --rm zpr-test
```

W celu sprawdzenia pokrycia kodu testami, preferowana metoda również opiera się 
na Docker'ze i korzysta z obrazu `zpr-test` wygenerowanego wcześniej.

W celu uruchomienia testu pokrycia należy przejść do katalogu `./atomic/test-coverage/` 
i z jego poziomu zbudować obraz Docer'a:
```bash
docker build -t zpr-coverage .
```
Po zbudowaniu obrazu można uruchomić test pokrycia:
```bash
docker run --rm zpr-coverage
``` 
W kontenerze zostaną najpierw wykonane wszystkie testy, a następnie zostanie 
zbadane pokrycie kodu, przy użyciu programu [gcovr](https://gcovr.com/en/stable/).
