xPath
##### syntax

- `nodename` --  select all nodes with name `nodename` | create collection with this name
- `/` -- select from the root
- `//` -- selec
- `.` -- select current node
- `..`-- select parent of current
- `@` -- select attributes

В XML узлы с одним и тем же `nodename` образуют коллекцию. 

Примечание: элементы из разных коллекций могут образовывать отношения "родитель-ребенок" (ещё по лаб1)

Поэтому запрос типа
- `/messages/note[1]` -- 
