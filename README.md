Тулза, как было задано, принимает 4 параметра, но я добавил еще один необязательный, для удобства: чтобы был вывод смещения замены и того, какие байты замещаются. 
Запуск без параметров выдаст Usage текст.
```
	$ ./Censor
	Usage:
	$ ./Censor input_file  output_file replacements_file restrictions_file [report]

	report - is any char, 
	if [report] presence - report while replace
	else - silence replace
	Program will print "Done" at the end
```
Для генерации большого файла я написал код на Golang (см. filegen.go). Сначала генерится файл с заданным числом слов, потом для скорости заданное количество раз переписывется в новый. 
Если нужна многобайтная кодировка - конвертирую сгенерёный файл в нужную кодировку с помощью GEdit или SublimeText, а затем уже копирую несколько раз в большой.

Словари писал без генерации - набор латинских букв. 

Проверять на правильность замены большой файл, оказывается, не так-то просто :)
Утилита diff воспринимает utf16 файлы как бинарные, cmp выводит в неудобном виде. Интернет подсказал такой метод:
```
	$ xdd input_file > hi
	$ xdd output_file > ho
	$ diff hi ho > diffs
```
Осторожно! Файлы получаются больше исходных. Файл diffs просматривал в mc с помощью F3. Проверял случайные смещения в hex-редакторе.

По поводу исполнения. Писал в qtCreator, но библиотеки qt не использовал.
Ранее с кодировками не работал, поэтому возникло желание сделать всё самому, на низком уровне, без использования библиотек для работы с кодировками.