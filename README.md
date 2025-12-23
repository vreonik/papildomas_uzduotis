# Aprašymas

Programa analizuoja tekstinį failą ir naudoja asociatyvius konteinerius (`std::map`, `std::set`) bei `std::string`, kad:

- suskaičiuotų, kiek kartų pasikartoja kiekvienas žodis;
- parodytų, kuriose teksto eilutėse tie žodžiai buvo paminėti;
- rastų ir išvestų visus tekste esančius URL adresus.

Programa veikia su bet kokiu tekstiniu (`.txt`) failu, kurį pateikia vartotojas.

## Funkcionalumas

Programa:

- skaito tekstą iš `.txt` failo;
- ignoruoja skyrybos ženklus;
- normalizuoja žodžius į mažąsias raides;
- palaiko lietuviškas raides (ą č ę ė į š ų ū ž);
- neskaičiuoja žodžių, kurie pasikartojo tik vieną kartą;
- sukuria cross-reference lentelę su eilučių numeriais;
- aptinka URL adresus (pilnus ir sutrumpintus);
- leidžia pasirinkti rikiavimą:
  - pagal abėcėlę;
  - pagal pasikartojimų dažnį.

Programa nelūžta, jei tekste yra UTF-8 simbolių (pvz., kitų kalbų raidžių).

## Naudojami konteineriai

- `std::map<std::string, int>` – žodžių pasikartojimų skaičiavimui
- `std::map<std::string, std::set<int>>` – eilučių numerių saugojimui
- `std::set<std::string>` – unikaliems URL adresams
- `std::string` – teksto apdorojimui


## Kompiliavimas
g++ -std=c++17 main.cpp -o programa

## Paleidimas
./programa

## Autorius:
-Veronika Rimkevičiūtė
