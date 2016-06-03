import QtQuick 2.4
import QtQuick.Window 2.2
//Подключаем библиотеку с компоновщиками
import QtQuick.Layouts 1.1
//Подключаем библиотеку с элементами управления
import QtQuick.Controls 1.4
//Подключаем библиотеку с диалоговыми окнами
import QtQuick.Dialogs 1.2

import Lab2.DBManager 1.0;
import Lab2.Repository 1.0;

Window {
    visible: true
    width: 420
    height: 190

    property int stateDlg: dlgAddEdit.stateDlg

    Repository {
        id: repository
        manager: DBManager {
            databaseName: "Z:/Lab2/Lab2.db"
        }
    }

    GridLayout {
        //  размером в 6 строк и 2 столбца
        rows: 7; columns: 2;
        //Определяем метку
        Text {
            // в ячейке 1, 1
            Layout.row: 1; Layout.column: 1
            //Задаем текст метки
            text: "id:"
        }
        //Определяем прямоугольник
        Rectangle {
            //  в ячейке 1, 2
            Layout.row: 1; Layout.column: 2
            //Задаем размеры прямоугольника
            width: 300; height: 20
            //Задаем цвет прямоугольника
            color: "grey"
            //Помещаем в прямоугольник метку, в которой будет отображаться
            //  информация из поля id текущей записи
            Text {
                //Выравниваем метку по вертикали относительно центра
                //  прямоугольника
                anchors.verticalCenter: parent.verticalCenter
                //Связываем свойство text метки со свойством recId элемента
                //  repository, через который получаем данные из БД
                text: repository.recId
            }
        }
        //Далее, по аналогии описываем другие элементы для вывода
        //  информации из текущей записи БД
        Text {
            Layout.row: 2; Layout.column: 1
            text: "Автор:"
        }
        Rectangle {
            Layout.row: 2; Layout.column: 2
            width: 300; height: 20
            color: "grey"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: repository.recAuthor
            }
        }
        Text {
            Layout.row: 3; Layout.column: 1
            text: "Название:"
        }
        Rectangle {
            Layout.row: 3; Layout.column: 2
            width: 300; height: 20
            color: "grey"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: repository.recName
            }
        }
        Text {
            Layout.row: 4; Layout.column: 1
            text: "Год выхода:"
        }
        Rectangle {
            Layout.row: 4; Layout.column: 2
            width: 300; height: 20
            color: "grey"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: repository.recYear
            }
        }
        Text {
            Layout.row: 5; Layout.column: 1
            text: "Количество страниц:"
        }
        Rectangle {
            Layout.row: 5; Layout.column: 2
            width: 300; height: 20
            color: "grey"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: repository.recPages
            }
        }
        //Описываем кнопку "<" для перемещения к предыдущей записи
        Button {
            //  в ячейке 5, 1
            Layout.row: 6; Layout.column: 1
            //Устанавливаем надпись на кнопку
            text: "<"
            //Описываем обработчик сигнала clicked()
            onClicked: {
                //Вызываем метод prev() элемента repository, который выполняет
                //  перемещение к предыдущей записи
                repository.prev()
            }
        }
        //Описываем кнопку ">" для перемещения к следующей записи
        Button {
            //  в ячейке 5, 2
            Layout.row: 6; Layout.column: 2
            text: ">"
            onClicked: {
                //Вызываем метод next() элемента repository, который выполняем
                //  перемещение к следующей записи
                repository.next()
            }
        }
        //Описываем компоновщик для размещения оставшихся кнопок
        //  в одну строку
        Row {
            //Компоновщик помещаем в ячейку 6,1
            Layout.row: 7; Layout.column: 1
            //  и он будет захватывать две ячейки
            Layout.columnSpan: 2
            //Описываем кнопку "Добавить" для добавления новой записи
            Button {
                text: "Добавить"
                onClicked: {
                    //Вызываем диалоговое окно, в которое будут вводиться
                    //  данные новой записи
                    dlgAddEdit.add()
                }
            }
            //Описываем кнопку "Редактировать" для редактирования
            //  текущей записи
            Button {
                text: "Редактировать"
                onClicked: {
                    //Вызываем диалоговое окно, в котором будет редактироваться
                    //  текущая запись, в метод edit передаем данные текущей
                    //  записи, полученные через элемент dtatBase
                    dlgAddEdit.update(repository.recName, repository.recAuthor,
                                      repository.recPages, repository.recYear, repository.recId)
                }
            }
            //Описываем кнопку "Удалить" для удаления текущей записи
            Button {
                text: "Удалить"
                onClicked: {
                    //Вызываем метод del() элемента repository, который выполняет
                    //  удаление текущей записи
                    repository.remove(repository.recId);
                }
            }
        }
    }
    //Описываем обработчик сигнала, возникающего при изменении свойства
    //  stateDlg. Это делается по той причине, что при вызове
    //  диалогового окна не происходит остановка программы до тех пор
    //  пока окно не закроется, поэтому состояние окна отслеживаем
    //  через специальное свойство
    onStateDlgChanged: {
        //Если свойство имеет значение 1
        if (stateDlg === 1)
            //  значит окно закрылось нажатием на кнопку "Ok"
            //  и было в режиме добавления
            //Добавляем новую запись вызовом метода add() элемента repository,
            //  в метод передаем данные из соответствующих свойств
            //  диалогового окна dlgAddEdit
            repository.add(dlgAddEdit.dlgName, dlgAddEdit.dlgAuthor,
                           dlgAddEdit.dlqPages, dlgAddEdit.dlgYear)
        //Если свойство имеет значение 2
        if (stateDlg === 2)
            //  значит окно закрылось нажатием на кнопку "Ok" и было в
            //  режиме редактирования
            //Редактируем текущую запись вызовом метода update()
            //  элемента repository
            repository.update(dlgAddEdit.dlgId, dlgAddEdit.dlgName, dlgAddEdit.dlgAuthor,
                              dlgAddEdit.dlgPages, dlgAddEdit.dlgYear)
    }
    //Описываем диалоговое окно Добавления/Редактирования записи. Окно
    //  может открываться в одном из двух режимов: Добавления - в этом
    //  случае полях ввода пустые и есть возможность ввода
    //  идентификатора, Редактирования - в полях ввода отображаются
    //  данные текущей записи, поле идентификатора заблокировано
    Dialog {
        //Определяем идентификатор окна
        id: dlgAddEdit
        //Определяем ширину окна
        width: 200
        //Описываем свойства, через которые передаем информацию из окна
        property int dlgId: -1
        property string dlgAuthor: ""
        property string dlgName: ""
        property int dlgYear: 0
        property int dlgPages: 0
        //Описываем свойство, хранящее состояние окна
        property int stateDlg: 0
        //Описываем вспомогательное свойство
        property int st
        //Описываем функцию add(), которая открывает окно
        //  в режиме добавления
        function add() {
            //Меняем заголовок окна
            title = "Добавление"
            //Разблокируем поле ввода tfId для ввода идентификатора
            //Очищаем поля ввода
            tfName.text = ""
            tfAuthor.text = ""
            tfPage.text = 0;
            tfYear.text = 0;
            //Открываем диалоговое окно
            this.open()
            //Задаем состояние окна - открытое
            stateDlg = 0
            //Указываем, что окно открыто в режиме добавления
            st = 1
        }
        //Описываем функцию edit(), которая открывает окно в режиме
        //  редактирования, в функцию передаются данные текущей записи
        function update(name, author, pages , year, id) {
            //Меняем заголовок окна
            title = "Редактирование"
            //Блокируем поле ввода tfId, т.к. идентификатор изменять нельзя

            //Остальным полям ввода присваиваем данные текущей записи
            tfName.text = name
            tfAuthor.text = author
            tfPage.text = pages
            tfYear.text = year
            dlgId = id
            //Открываем диалоговое окно
            this.open()
            //Задаем состояние окна - открытое
            stateDlg = 0
            //Указываем, что окно открыто в режиме редактирования
            st = 2
        }
        //Описываем кнопки, отображаемые в окне Ok и Cancel
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        //В диалоговом окне размещаем табличный компоновщик
        GridLayout {
            //  размером в 4 строки и 2 столбца
            rows: 4; columns: 2;
            //Определяем метку
            Text {
                // в ячейке 1, 1
                Layout.row: 1; Layout.column: 1
                //Задаем текст метки
                text: "Название:"
            }
            //Описываем поле ввода
            TextField {
                // в ячейке 1, 2
                Layout.row: 1; Layout.column: 2
                //Определяем идентификатор поля
                id: tfName
                //Описываем замещающий текст
                placeholderText: "Укажите название"
            }
            //Далее, по аналогии описываем другие элементы для
            //  ввода информации
            Text {
                Layout.row: 2; Layout.column: 1
                text: "Автор:"
            }
            TextField {
                id: tfAuthor
                Layout.row: 2; Layout.column: 2
                placeholderText: "Укажите автора"
            }
            Text {
                Layout.row: 3; Layout.column: 1
                text: "Количество страниц:"
            }
            TextField {
                id: tfPage
                Layout.row: 3; Layout.column: 2
                placeholderText: "Укажите количество страниц"
            }
            Text {
                Layout.row: 4; Layout.column: 1
                text: "Год выхода:"
            }
            TextField {
                id: tfYear
                Layout.row: 4; Layout.column: 2
                placeholderText: "Укажите год выхода"
            }
        }
        //Описываем обработчик сигнала accepted(), возникающего
        //  при нажатии на кнопку Ok
        onAccepted: {
            //Передаем значения полей ввода в соответствующие свойства
            //  диалогового окна
            dlgName = tfName.text
            dlgAuthor = tfAuthor.text
            dlgPages = Number(tfPage.text)
            dlgYear = Number(tfYear.text)
            //Передаем в свойство stateDlg состояние окна:
            //  1 – добавление или 2 - редактирование
            stateDlg = st
        }
        //Описываем обработчик сигнала rejected(), возникающего
        //  при нажатии на кнопку Cancel
        onRejected: {
            //Передаем в свойство stateDlg состояние окна:
            //  -1 - закрыто нажатием на кнопку Cancel
            stateDlg = -1
        }
    }
}




