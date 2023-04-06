## PROCESS
 
В XML узлы с одним и тем же `node` образуют коллекцию. 
Коллекция подчиняется схеме 
Примечание: элементы из разных коллекций могут образовывать отношения "родитель-ребенок" (ещё по лаб1)

SLASH -- AST-separator
##### queries
Запросы с оператором
- [x]  `node/child_node[1]` -- выбрать из узла схемы `node` 1-го потомка схемы `child_node` 
- [x] `node/child_node[1][@attribute =|!=| > | < num|str] ` 
- [x] `node/child_node[@attribute =|!=| > | < num|str][1]` 
- [x] `node[child_node]` done select node with 1 or more child_node done
- [x] `nodename` or  -- select all nodes with this name (== read collection)
- [x] `node/@attribute` -- child-nodes of node with attr==attribute
- [x] `/node/*` -- Выбирает все элементы, которые являются прямыми потомками элемента messages==node

- [ ] `/node` -- выбрать все узлы схемы нода
- [ ] `/node/foo` -- выбрать ноды foo, которые являются потомками node


- [ ] `update(/node/foo[1][@attribute=value][@attribute=value])`
- [ ] `remove(path)`
- [ ] `create_element(/node/foo[1][@attribute=value][@attribute=value]))`
- [ ] `create_schema(path)` -- путь как будто работаем в `xsd`

TODO (queries)
- `//*`-- print all db **TODO** or maybe not
- `node/[child1node | child2node ] ` вообще в лабе такое не предусматривается (ну на сервере)
- `//node` -- select all nodes (вне зависимости от отношений) **THINK**
- `parent//node` -- select all node==child (вне зависимости от уровня вложенности) на серваке такого не реализовано
- `//@attribute` -- select all nodes with attr==attribute 
- `//node1 | //node2` -- select col1 AND col2 from db 
- operator with substring **ADD**

- (?) functions between queries *optional*
- functions like: like(), text() -- *optional*

** НЕ ЗАБЫТЬ ПРОВЕРИТЬ**
- поддержка логической комбинации??

Примечания.
1. Операция выборки ноды со всеми узлами не реализована (хотя в xPath) присутствует, т.к. на сервере запрос выборки ноды уже подразумевает вывод всех ее атрибутов (не только отношений на уровне док. дерева)
