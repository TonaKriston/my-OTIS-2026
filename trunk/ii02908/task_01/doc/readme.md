<p align="center">Министерство образования Республики Беларусь</p>
<p align="center">Учреждение образования</p>
<p align="center">«Брестский Государственный технический университет»</p>
<p align="center">Кафедра ИИТ</p>

<br><br><br><br><br>

<p align="center"><b>Лабораторная работа №1</b></p>
<p align="center">по дисциплине «Общая теория интеллектуальных систем»</p>
<p align="center"><b>Тема: «Моделирование температуры объекта»</b></p>

<br><br><br>

<p align="right">Выполнила:</p>
<p align="right">студентка 2 курса</p>
<p align="right">группы ИИ-29</p>
<p align="right">Затычиц К. В.</p>

<p align="right">Проверил:</p>
<p align="right">Дворанинович Д. А.</p>

<br><br><br><br><br>

<p align="center">Брест 2026</p>

---------------------------------------------------------
Вариант 27

- линейная модель 1.3 y_{\tau+1} = a_1*y{\tau} + a_2*y{\tau-1} + b*u_{\tau}
- нелинейная модель 2.10 y_{\tau+1} = a*tanh(y_{\tau}) + b*u_{\tau}^3
- дифференциальная модель 3.2 dy/dt = b*u


Для выполнения задания мною были использованы навыки ООП. Создан абстрактный базовый класс Model с виртуальным методом step,
который запускает итерационный процесс подсчёта уравнений.

Готовая программа может выполнять заданное пользователем количество итераций. Коэффициенты уравнения пользователь
также может ввести сам. Результаты сохраняются в .csv файлы, а также с помощью программы gnuplot они преобразовываются в
.png изображения графиков (gnuplot должен быть предустановлен на устройстве пользователя).

Сборка исполняемой программы осуществляется файлом build.bat. После сборки запускать программу нужно через run.bat (запуск напрямую
.exe из каталога build будет работать некорректно, в виду расположения необходимых для построения графиков .gp файлов).
----------------------------------------------------------
Демонстрация сборки:
<img width="1459" height="703" alt="image" src="https://github.com/user-attachments/assets/344c31a0-1c73-4d51-b734-fedabaeb55b3" />


Демонстрация работы программы:
<img width="929" height="423" alt="image" src="https://github.com/user-attachments/assets/289b2ec9-9b94-44fd-bf1d-9d50aa55bd37" />



Пример содержания готового .csv файла:
(первый столбец - счётчик количества итераций)

<img width="189" height="450" alt="image" src="https://github.com/user-attachments/assets/21be4f82-f606-4176-b208-702939fa65a9" />


Пример готовых графиков:
<img width="1000" height="700" alt="plot_dif_harmonic" src="https://github.com/user-attachments/assets/047f3c69-588c-4f0b-9256-8ab32272f961" />
<img width="1000" height="700" alt="plot_nonlinear_impulse" src="https://github.com/user-attachments/assets/4a48f912-9a7f-4f3e-8bbb-24670636ec24" />
<img width="1000" height="700" alt="plot_dif_step" src="https://github.com/user-attachments/assets/469e6ba0-5e8c-4d86-8fc1-81d7d8074c09" />


Отзывы на чужие работы:
<img width="1403" height="691" alt="image" src="https://github.com/user-attachments/assets/7cde472f-0e26-4be5-aaca-a4274d0676bd" />

<img width="1421" height="454" alt="image" src="https://github.com/user-attachments/assets/7b7f0c4d-e4b6-4d87-b92b-5b5ab572837f" />

<img width="1332" height="823" alt="image" src="https://github.com/user-attachments/assets/d1fc0e30-a521-430d-b2aa-efc0cd05acb4" />


