
Gdy programista uruchamia wybraną funkcję wykonywane są następujące kroki:
 1. Wypełniana jest struktura utworzona przez rpgen	danymi podanymi jako parametry funkcji.
 2. Wywoływana jest lokalna funkcja wygenerowana przez rpgen, której zadaniem jest uruchomienie odpowiadającej jej funkcji na serwerze.
 3. Zwracane jest sterowanie do programu.

Wynik przetwarzania funkcji zostanie przekazany do funkcji zwrotnej podanej przez programistę jako jeden z parametrów funkcji (parametr callback).

Po stronie serwera po odebraniu nowego żądania wykonywane są następujące kroki:
 1. Do klienta zwracany jest status informujący o przyjęciu zadania do przetworzenia.
 2. Uruchamiana jest funkcja przetwarzająca żadanie.
    2.1. Wykonywane jest zlecone zadanie
    2.2. Uruchamiany jest nowy wątek, którego zadaniem jest przesłanie wyników przetwarzania do mini-serwera po stronie klienta.
         Nowy wątek jest konieczny by nieprawidłowo wykonany klient odbierający wyniki nie blokował serwera (ma to miejsce, gdy funkcja klienta rozpocznie dalsze przetwarzanie przed zamknięciem połączenia).