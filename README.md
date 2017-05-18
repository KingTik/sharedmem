Aplikacja chat oparta na pamieci współdzielonej.

1. client
    - uruchomienie ./client <nazwa_uzytkownika>
    - nazwa uzytkownika moze miec do 14 znakow
    - glowny watek programu pobiera od uzytkownika wiadomosc i przesyla do servera
    - osobny watek czeka az na liscie wiadomosci wystwiwanych przez server jest dokonana zmiana (pojawia sie wiadomosc z nowym id). jezeli tak to czysci konsole i wyswietla cala liste
    - wiadomosci moga miec do 60 znakow dlugosci, w wprzypadku wiekszych zostanie podzielone na pczaki i wysylane osobno

2. server
    - czeka na nadejscie wiadomosci
    - jezeli nowa wiadomosc sie pojawi, ustawia w strukturze pole mowiace ze zostala ona odczytana, dodaje wiadomosc do listy 
    - nie trzyma informacji o tym ilu uzytkownikow jest w danym momencie 'podpietych' do chatu
    - trzyma w pamieci do 10 ostatnich wiadomosci 
    - nadaje wiadomosciom unikalny id.