### lab2
tools: xPath + flex&bison

##### brief task:
TODO:
- [x] lexer
- [x] parser
- [x] test program
- [ ] report

В XML узлы с одним и тем же `node` образуют коллекцию для документного дерева из 1-й лабы.
Важно:
- коллекция "подчинена" схема
- элементы из разных коллекций могут образовывать отношения "родитель-ребенок"
- операция выборки ноды со всеми узлами не реализована (хотя в xPath присутствует), т.к. на сервере запрос выборки ноды уже подразумевает вывод всех ее атрибутов (не только отношений на уровне док. дерева)

#### Примеры выполнения запросов.
- `/foo[@a=1]`
  ```
       descendant:
         ---> shema name: foo
        filter = select property:
         --> property name a
        operator:
        equal to
        value = 1
    ```
- `/foo[1][@a=1]`
  ```
   descendant:
    ---> shema name: foo
    filter = select:
    --> element number: 1
    filter = select property:
    --> property name a
    operator:
    equal to
    value = 1
    ````
- `/foo[child][@a=1]`
- ```
   descendant: 
    ---> shema name: foo
    filter = select:
    --> descendant schema name: child
    filter = select property:
    --> property name a
    operator:
    equal to
    value = 1
  ```
- `/foo[@a=1][@b=2]`
  ```
      -descendant:
           ---> shema name: foo
      filter = select property:
      --> property name a
      operator:
      equal to
      value = 1

      filter = select property:
      --> property name b
      operator:
      equal to
      value = 2
    ````
  - `/foo/@a`
    ```
     descendant:
         ---> shema name: foo

        --> node property: a
    ````
  - `foo`
    ```
      --> schema name: foo
    ```

    - `update(/foo[1][@a=1])` -- меняет 1-му элементу схемы foo значение атрибута a=1
      ```
        descendant:
             ---> shema name: foo
        filter = select:
        --> element number: 1
        filter = select property:
        --> property name a
        operator:
        equal to
        value = 1

        OPERATION: update
        ```
    - `remove_scheme(foo)` -- удалить схему foo (то есть коллекцию foo)
      ```
         --> schema name: foo
        OPERATION: remove_scheme
        ```
    - `remove_element(/foo[@a=1])` -- удалить из элементов схемы foo те, у которых значени атрибута(свойства) a=1
      ```
        descendant:
             ---> shema name: foo
        filter = select property:
        --> property name a
        operator:
        equal to
        value = 1

        OPERATION: remove_element
        ```
    - `create_scheme(foo[@attrname;type=int32][@attr;type=string])` -- создать коллекцию foo с заданными атрибутами и их типами
      ```
         --> schema name: foo
        property name: attrname
        property type:
        int32
        property name: attr
        property type:
        string
        OPERATION: create_scheme
        ```
    - `create_scheme(foo[@attrname;type=int32][@attr;type=string])` -- создать коллекцию foo с заданными атрибутами и их типами
      ```
         descendant:
         ---> shema name: foo
         descendant:
         ---> shema name: zoo
         filter = select property:
         --> property name a
         operator:
         equal to
           value = 1

         OPERATION: create_element
        ```
    - `*` -- выборка всей базы данных
    
      ``select all``
