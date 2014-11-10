Формат байт-кода dwarf-vm
=========================

Типы данных
-----------

Название    | Описание                              | Размер (байт)
----------- |---------------------------------------|--------------
int         | Целое число                           | 8
reg_num     | Номер регистра                        | 2
addr_offset | Смещение относительно счетчика команд | 4

Инструкции
----------

 Код | Мнемоника            | Описание
 --- | ----------------     | ---------------------------------
  00 | HALT                 | Останов программы
  01 | IADD                 | A = pop(); B = pop(); push(A + B)
  02 | ISUB                 | A = pop(); B = pop(); push(A - B)
  03 | IMUL                 | A = pop(); B = pop(); push(A * B)
  04 | IDIV                 | A = pop(); B = pop(); push(A / B)
  05 | DUP                  | A = peek(); push(A)
  06 | IPUSHC *NUM*:int       | push(NUM)
  07 | IPUSHREG *N*:reg_num   | push(RegN)
  08 | IPOPREG *N*:reg_num    | RegN = pop()
  09 | JMP *OFF*:addr_offset  | PC = PC + OFF
  0A | JZ *OFF*:addr_offset   | A = pop(); if (A == 0) { PC = PC + OFF }
  0B | JNZ *OFF*:addr_offset  | A = pop(); if (A != 0) { PC = PC + OFF }
  0C | ICMP                 | FIXME
  0D | EQ                   | A = pop(); B = pop(); if (A == B) push(1); else push(0);
  0E | PRINT                | Печатает число с верхушки стека на устройсве вывода
  0F | DEBUG                | FIXME
  
  **Примечание:** для инструкций перехода адрес вычисляется с использованием PC 
  а тот момент, когда инструкция полностью декодирована.
  
  Например, инструкция *jmp -5* вводит машину в бесконечный цикл (т.к. постоянно передает управление на начало инструкции), в то время как *jmp 0* ничего не делает.
  