-------------------------------------------------------------------------------
-- AUTOR:
-- Tomasz Wsuł
-- indeks 80169
-- 2nickers@gmail.com
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- Uruchomienie:
-------------------------------------------------------------------------------
W katalogu głównym uruchamiany plik './make.sh' który skompiluje źródła. 
Wewnątrz ./bin/ pojawią się 3 pliki: serwer, testowa aplikacja kliencka 
oraz biblioteka.


-------------------------------------------------------------------------------
-- Inicjalizacja:
-------------------------------------------------------------------------------
 Przed pierwszym użyciem jakiejkolwiek funkcji z bibliteki należy ją 
 zainicjalizować funkcją 'init_rf(char* host)', gdzie host to nazwa servera 
 na którym uruchomiony jest server RPC wraz z usługą.  

-------------------------------------------------------------------------------
-- Wykowanie funkcji:
-------------------------------------------------------------------------------
Gdy programista uruchamia wybraną funkcję wykonywane są następujące kroki:
 1. Wypełniana jest struktura utworzona przez rpgen danymi podanymi jako 
    parametry funkcji.
 2. Wywoływana jest lokalna funkcja wygenerowana przez rpgen, której zadaniem 
    jest uruchomienie odpowiadającej jej funkcji na serwerze.
 3. Zwracane jest sterowanie do programu.
 
 Wynik przetwarzania funkcji zostanie przekazany do funkcji zwrotnej podanej 
 przez programistę jako jeden z parametrów funkcji (parametr callback).

-------------------------------------------------------------------------------
-- Odebranie żądania:
-------------------------------------------------------------------------------
Po stronie serwera po odebraniu nowego żądania:
 1. Do klienta zwracany jest status informujący o przyjęciu zadania.
 2. Uruchamiana jest funkcja przetwarzająca żadanie.
    2.1. Wykonywane jest zlecone zadanie
    2.2. Uruchamiany jest nowy wątek, którego zadaniem jest przesłanie wyników
         przetwarzania do mini-serwera po stronie klienta.
         
         Nowy wątek jest konieczny by nieprawidłowo wykonany klient odbierający 
         wyniki nie blokował serwera (ma to miejsce, gdy funkcja klienta 
         rozpocznie dalsze przetwarzanie przed zamknięciem połączenia).
         
-------------------------------------------------------------------------------
-- Odebranie wyniku:
-------------------------------------------------------------------------------
Odbieranie wyników po stronie klienta:
 1. Wyniki są odczytywane
 2 Tworzony jest nowy wątek do uruchomienia funkcji zwrotnej użytkownika
    2.1. Tworzony jest nowy wątek
    2.2. uruchamiana jest funkcja zwrotna podana przez programistę (callback)
 3. W wątku, który odebrał wynik zwracane jest sterowanie do 
