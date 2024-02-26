Данный репозиторий является практической частью работы
"Сравнение DVMH-системы и XcalableACC" на примере тестирования распараллеливания программы Himeno.
Доктор Рютаро Химено, директор Передового центра вычислений и коммуникаций, разработал этот тест для оценки производительности кода анализа несжимаемых жидкостей. Эта эталонная программа выполняет измерения для выполнения основных циклов решения уравнения Пуассона с использованием итерационного метода Якоби.

Здесь представлены файлы:
1.   himenoInit.c    исходный код
2.   himenoXACC.c    распараллеленый с помощью XACC код
3.   himenoDVMH     распаралелленый с помощью DVMH код
4.   parametr.h     выбор размера для массивов

Оставляю к коду комментарии, чтоб некоторые моменты были понятны 
