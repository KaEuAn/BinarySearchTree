This project contains a data structure, which represents array supporting following operations:
1)Subtracting sum
2)Insert an element x into a position i
3)Delete an element with the index i
4)Assign value in subsection
5)Add value in subsection
6)Make next_permutation in subsection
7)Make next_permutation in subsection

В первой строке вводится количество элементов в массиве. Во второй - сами элементы. 
В третьей - количество операций, а затем сами операции в приведённом ниже виде.
По окончанию выполнения операций выводится конечный массив.

1) l r (0 ≤ l ≤ r < arraySize) - найти сумму всех чисел массива на отрезке [l, r];
2) x pos (|x| ≤ 3*10^4, 0 ≤ pos ≤ arraySize) - вставить элемент x на позицию pos;
3) pos (0 ≤ pos < arraySize) - удалить элемент x, находящийся на позиции pos;
4) x l r (|x| ≤ 3*10^4, 0 ≤ l ≤ r < arraySize) - присвоить элементам на подотрезке [l, r] число x;
5) x l r (|x| ≤ 3*10^4, 0 ≤ l ≤ r < arraySize) - прибавить к элементам на подотрезке [l, r] число x;
6) l r - выполнить next_permutation на подотрезке [l, r];
7) l r - выполнить prev_permutation на подотрезке [l, r].
