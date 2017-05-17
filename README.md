1. client
    - uruchomienie ./client <user_name>
    - glowny watek programu pobiera od uzytkownika wiadomosc i przesyla do servera
    - osobny watek czeka az na liscie wiadomosci wystwiwanych przez server jest dokonana zmiana. jezeli tak to czysci konsole i wyswietla cala liste

2. server
    - czeka na nadejscie wiadomosci
    - jezeli nowa wiadomosc sie pojawi, ustawia w strukturze pole mowiace ze zostala ona odczytana, dodaje wiadomosc do listy 